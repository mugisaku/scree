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


Token
process_defined_operator(Cursor&  cur, Context const&  ctx)
{
    if(isalpha(*cur) || (*cur == '_'))
    {
      auto  id = read_identifier(cur);

        if(ctx.find_macro(id))
        {
          return Token(TokenKind::decimal_integer,std::string("1"));
        }
    }

  else
    if(*cur == '(')
    {
    }

  else
    {
      throw Error(cur,"defined演算子の後に有効な要素が無い");
    }


  return Token(TokenKind::decimal_integer,std::string("0"));
}


TokenString
process_identifier(Token&&  id, Cursor&  cur, Context const&  ctx)
{
  TokenString  ls;

  auto  macro = ctx.find_macro(*id);

    if(macro)
    {
      auto&  text = macro->get_text();

        if(macro->is_function_style())
        {
          skip_spaces_and_newline(cur);

            if(*cur != '(')
            {
              throw Error(cur,"関数式マクロの引数リストが無い");
            }


          cur += 1;

          auto  argls = read_argument_list(cur);

            if(!macro->test_number_of_arguments(argls))
            {
              throw Error(Cursor(),"引数の数が一致しません");
            }



          ls = process_text(text,ctx,&argls);
        }

      else
        {
          ls = process_text(text,ctx);
        }
    }

  else
    {
      ls.emplace_back(std::move(id));
    }


  return std::move(ls);
}


TokenString
process_file(std::string const&  s, Context&  ctx)
{
  Cursor  cur(s);

  TokenString  toks;

    if(*cur == '#')
    {
      cur += 1;

      auto  s = read_directive(cur);

      toks = process_directive(Cursor(s),ctx);
    }


    for(;;)
    {
        if(cur.compare('\n','#'))
        {
          cur += 1;

          cur.newline();

          auto  s = read_directive(cur);

          toks = process_directive(Cursor(s),ctx);
        }

      else
        if(cur.compare('\n'))
        {
          cur.newline();
        }

      else
        {
          skip_spaces(cur);

            if(!*cur)
            {
              break;
            }

          else
            {
              auto  tok = read_token(cur);

                if(!tok)
                {
                  break;
                }

              else
                if(!ctx.test_locked_flag())
                {
                    if(tok == TokenKind::identifier)
                    {
                      toks += process_identifier(std::move(tok),cur,ctx);
                    }

                  else
                    {
                      toks.emplace_back(std::move(tok));
                    }
                }
            }
        }
    }


  toks.emplace_back(Token());

  return std::move(toks);
}


TokenString
process_text(std::string const&  s, Context const&  ctx, ArgumentList const*  argls)
{
  Cursor  cur(s);

  TokenString  toks;

    for(;;)
    {
      skip_spaces_and_newline(cur);

        if(!*cur)
        {
          break;
        }

      else
        if(cur.compare('#','#'))
        {
        }

      else
        if(cur.compare('#'))
        {
        }

      else
        {
          auto  tok = read_token(cur);

            if(!tok)
            {
              break;
            }

          else
            if(!ctx.test_locked_flag())
            {
                if(tok == TokenKind::identifier)
                {
                  toks += process_identifier(std::move(tok),cur,ctx);
                }

              else
                {
                  toks.emplace_back(std::move(tok));
                }
            }
        }
    }


  toks.emplace_back(Token());

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
      toks = preprocessing::process_file(s,ctx);
    }


    catch(preprocessing::Error&  e)
    {
      e.cursor.print();

      printf("%s\n",e.what());
    }


  toks.print();
  ctx.print();
  

  return 0;
}



