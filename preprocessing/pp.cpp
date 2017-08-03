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
process_file(std::string const&  s)
{
  Cursor  cur(s);

  TokenString  toks;

    if(*cur == '#')
    {
      cur += 1;

      toks += Token(TokenKind::directive,read_directive(cur));
    }


    for(;;)
    {
        if(cur.compare('\n','#'))
        {
          cur += 1;

          cur.newline();

          toks += Token(TokenKind::directive,read_directive(cur));
        }

      else
        if(cur.compare('\n'))
        {
          cur.newline();
        }

      else
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
    }


  return std::move(toks);
}


}


int
main(int  argc, char**  argv)
{
  auto  s = load_file(argv[1]);

  preprocessing::Context  ctx;

  preprocessing::TokenString  toks;

    try
    {
      toks = preprocessing::process_file(s);

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



