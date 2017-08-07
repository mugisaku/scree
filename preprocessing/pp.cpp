#include<string>
#include<cstdlib>
#include<cstring>
#include<cctype>
#include<vector>
#include<getopt.h>
#include"pp.hpp"


namespace preprocessing{


std::string
read_directive(Cursor&  cur)
{
  std::string  s;

    for(;;)
    {
      auto  const c = *cur;

        if(c == '\n')
        {
          break;
        }

      else
        if((c ==  ' ') ||
           (c == '\t') ||
           (c == '\r'))
        {
          cur += 1;

          s.push_back(' ');
        }

      else
        if(cur.compare("\\\n"))
        {
          cur += 1;

          cur.newline();
        }

      else
        if(iscntrl(c))
        {
          throw Error(cur,"ディレクティブの途中で制御文字");
        }

      else
        {
          cur += 1;

          s.push_back(c);
        }
    }


  return std::move(s);
}


TokenString
process_identifier(Token const&  id, TokenString::const_iterator&  it, Context const&  ctx, Macro const*  parent)
{
    if(*id == "__FILE__")
    {
      auto  tok = Token(TokenKind::string,std::string(id.get_info().get_file_path()));

      return TokenString(std::move(tok));
    }

  else
    if(*id == "__LINE__")
    {
      auto  tok = Token(TokenKind::decimal_integer,std::to_string(id.get_info().get_line_count()));

      return TokenString(std::move(tok));
    }


  auto  macro = ctx.find_macro(*id);

    if(macro && (macro != parent))
    {
        if(macro->is_function_style())
        {
            if(*it != '(')
            {
              throw Error(Cursor(),"%sは関数マクロですが、実引数リストがありませsん",macro->get_name().data());
            }


          it += 1;

          auto  args = read_argument_list(it,ctx);

          return macro->expand(ctx,&args);
        }

      else
        {
          return macro->expand(ctx,nullptr);
        }
    }


  return TokenString();
}


TokenString
process_text(std::string const&  s)
{
  Cursor  cur(s);

  TokenString  toks;

    for(;;)
    {
      skip_spaces(cur);

      auto  tok = read_token(cur);

        if(!tok)
        {
          break;
        }

      else
        {
          toks += std::move(tok);
        }
    }


  return std::move(toks);
}


TokenString
process_file(std::string const&  s, std::string*  file_path)
{
  Cursor  cur(s,file_path);

  TokenString  toks;

    if(*cur == '#')
    {
      TokenInfo  info(cur);

      cur += 1;

      toks += Token(TokenKind::directive,read_directive(cur),std::move(info));
    }


    while(*cur)
    {
        if(cur.compare('\n','#'))
        {
          cur.newline();

          TokenInfo  info(cur);

          cur += 1;

          toks += Token(TokenKind::directive,read_directive(cur),std::move(info));
        }

      else
        if(cur.compare('\n'))
        {
          cur.newline();
        }

      else
        {
          skip_spaces(cur);

          toks += read_token(cur);
        }
    }


  return std::move(toks);
}


}


namespace{


FILE*   in;
FILE*  out;

char const*  in_path;

bool  quiet;


std::string
load_file(FILE*  f)
{
  std::string  s;

    if(f)
    {
        for(;;)
        {
          auto  c = fgetc(f);

            if(feof(f) || ferror(f))
            {
              break;
            }


          s.push_back(c);
        }


      fclose(f);
    }


  return std::move(s);
}


void
process_option(int  argc, char**  argv, preprocessing::Context&  ctx)
{
  static struct option  const options[] = {
    {"help",no_argument,nullptr,'h'},
    {"quiet",no_argument,nullptr,'q'},
  };


  std::vector<std::string>  u_list;

    for(;;)
    {
      auto  c = getopt_long(argc,argv,"D::U:I:",options,nullptr);

        switch(c)
        {
      case('D'):
          {
            char   name[256]      ;
            char  value[256] = "1";

            auto  res = sscanf(optarg,"%256s=%256s",name,value);

              if(res >= 1)
              {
                std::string  s(name);

                preprocessing::Macro  m(std::move(s));


                std::string  vs(value);

                m.set_token_string(preprocessing::process_text(vs));

                ctx.append_macro(std::move(m));
              }
          }
          break;
      case('U'):
          {
            char  name[256];

              if(sscanf(optarg,"%256s",name) == 1)
              {
                u_list.emplace_back(name);
              }
          }
          break;
      case('I'):
          {
            char  path[256];

              if(sscanf(optarg,"%256s",path) == 1)
              {
                ctx.append_include_directory(std::string(path));
              }
          }
          break;
      case('h'):
          printf("使い方: pp {オプション | [入力ファイルパス]} [出力ファイルパス]\n");
          printf("オプション\n");
          printf("-Dname[=value]  valueをnameという名前で定義する\n");
          printf("-Uname          nameという名の定義を消す\n");
          printf("-Ipath          インクルードファイルを探す先に、pathを追加する\n");
          printf("入力ファイルパスに \"--\" が渡されるか省略された場合、標準入力から読み込む\n");
          printf("出力ファイルパスに \"--\" か渡されるか省略された場合、標準出力へ書き込む\n");
          printf("\n");
          exit(0);
          break;
      case('q'):
          quiet = true;
          break;
      case('?'):
          break;
      case(-1):
            for(int  i = optind;  i < argc;  ++i)
            {
              std::string  arg(argv[i]);

                if(arg == "--")
                {
                    if(!in){ in =  stdin;}
                  else     {out = stdout;}
                }

              else
                {
                  auto  path = arg.data();

                    if(!in)
                    {
                      in_path = path;

                      in = fopen(path,"rb");

                        if(!in)
                        {
                          printf("入力ファイル%sを開けませんでした\n",path);

                          exit(-1);
                        }
                    }

                  else
                    {
                      out = fopen(path,"wb");

                        if(!out)
                        {
                          printf("出力ファイル%sを開けませんでした\n",path);

                          exit(-1);
                        }
                    }
                }
            }


            if(!in)
            {
              in = stdin;
            }


            if(!out)
            {
              out = stdout;
            }


            for(auto&  s: u_list)
            {
              ctx.remove_macro(s);
            }
          return;
        }
    }

}


}


int
main(int  argc, char**  argv)
{
  preprocessing::Context  ctx;

  preprocessing::TokenString  toks;


  process_option(argc,argv,ctx);


  auto  s = load_file(in);

    try
    {
      toks = preprocessing::process_file(s,new std::string(std::move(in_path)));

        if(!quiet)
        {
          printf("file processing is end\n");
        }


      toks = preprocessing::process_token_string_that_includes_directives(std::move(toks),ctx);

        if(!quiet)
        {
          printf("token string processing is end\n");
        }
    }


    catch(preprocessing::Error&  e)
    {
      e.cursor.print();

      printf("%s\n",e.what());
    }


  toks.print(out,true);

    if(!quiet)
    {
      ctx.print();
    }


  return 0;
}



