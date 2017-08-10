#include<string>
#include<cstdlib>
#include<cstring>
#include<cctype>
#include<vector>
#include<getopt.h>
#include"pp.hpp"


namespace{


FILE*   in;
FILE*  out;

std::string  in_path;

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

            if(feof(f))
            {
              break;
            }


            if(ferror(f))
            {
              printf("ファイル読み込み中にエラー");

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


                m.set_token_string(preprocessing::tokenize_sub_text(value));

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
          printf("使い方: pp {オプション} 入力ファイルパス [出力ファイルパス]\n");
          printf("オプション\n");
          printf("-Dname[=value]  valueをnameという名前で定義する\n");
          printf("-Uname          nameという名の定義を消す\n");
          printf("-Ipath          インクルードファイルを探す先に、pathを追加する\n");
          printf("入力ファイルパスに \"-\" が渡された場合、標準入力から読み込む。省略できない\n");
          printf("出力ファイルパスに \"-\" か渡されるか省略された場合、標準出力へ書き込む\n");
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

                if(arg == "-")
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

  ctx.append_include_directory(std::string("../preprocessing"));
  ctx.append_include_directory(std::string("/usr/include"));
  ctx.append_include_directory(std::string("/usr/include/c++/5"));
  ctx.append_include_directory(std::string("/usr/include/i386-linux-gnu"));
  ctx.append_include_directory(std::string("/usr/include/i386-linux-gnu/c++/5"));


  preprocessing::TokenString  toks;


  process_option(argc,argv,ctx);


  auto  s = load_file(in);

    try
    {
      toks = preprocessing::tokenize_main_text(s.data(),in_path.data());

        if(!quiet)
        {
          printf("file processing is end\n");
        }


      preprocessing::process_token_string_that_includes_directives(toks,ctx);

        if(!quiet)
        {
          printf("token string processing is end\n");
        }
    }


    catch(preprocessing::Error&  e)
    {
      e.cursor.print();

      printf("%s\n",e.what());

      exit(-1);
    }


  toks.print(out,true);

    if(!quiet)
    {
      ctx.print();
    }


  return 0;
}



