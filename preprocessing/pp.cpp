#include<string>
#include<cstdlib>
#include<cstring>
#include<cctype>
#include<vector>
#include"pp.hpp"


namespace{


std::string
load_file(const char*  path)
{
  auto  f = fopen(path,"rb");

  std::string  s;

    if(f)
    {
        for(;;)
        {
          auto  c = fgetc(f);

            if(feof(f))
            {
              break;
            }


          s.push_back(c);
        }


      fclose(f);
    }


  return std::move(s);
}


}


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


int
main(int  argc, char**  argv)
{
  auto  path = argv[1];

  auto  s = load_file(path);

  preprocessing::Context  ctx;

  preprocessing::TokenString  toks;

    try
    {
      toks = preprocessing::process_file(s,new std::string(path));

printf("file processing is end\n");

      toks = preprocessing::process_token_string_that_includes_directives(std::move(toks),ctx);

printf("token string processing is end\n");
    }


    catch(preprocessing::Error&  e)
    {
      e.cursor.print();

      printf("%s\n",e.what());
    }


printf("**processed**\n");
  toks.print();
printf("**context**\n");
  ctx.print();
  

  return 0;
}



