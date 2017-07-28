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


TokenString
process_identifier(Token&&  id, Cursor&  cur, Context&  ctx)
{
  TokenString  ls;

  auto  macro = ctx.find_macro(id->string);

    if(macro)
    {
        if(macro->is_function_style())
        {
          skip_spaces_and_newline(cur);

            if(*cur != '(')
            {
              throw Error(cur,"関数式マクロの引数リストが無い");
            }


          cur += 1;

          auto  argls = read_argument_list(cur);

          auto  s = macro->replace_text(argls);

          ls = process_main(Cursor(s),ctx);
        }

      else
        {
          ls = process_main(Cursor(macro->get_text()),ctx);
        }
    }

  else
    {
      ls.emplace_back(std::move(id));
    }


  return std::move(ls);
}


Token
read_token(Cursor&  cur)
{
  Token  tok;

  skip_spaces_and_newline(cur);

  auto  const c = *cur;

    if(c)
    {
        if(c == '\'')
        {
          cur += 1;

          tok = read_character_literal(cur);
        }

      else
        if(c == '\"')
        {
          cur += 1;

          tok = read_string_literal(cur);
        }

      else
        if(cur.compare('/','*'))
        {
          cur += 2;

          skip_blockstyle_comment(cur);
        }

      else
        if(cur.compare('/','/'))
        {
          cur += 2;

          skip_linestyle_comment(cur);
        }

      else
        if(c == '0')
        {
          cur += 1;

          tok = read_number_literal_that_begins_by_zero(cur);
        }

      else
        if((c >= '1') &&
           (c <= '9'))
        {
          tok = read_decimal_number_literal(cur);
        }

      else
        if(isalpha(c) || (c == '_'))
        {
          tok = read_identifier(cur);
        }

      else
        {
    printf("%c\n",c);
//          throw Error(cur,"処理不可の文字");
cur += 1;
        }
    }


  return std::move(tok);
}


namespace{
char const* 
const keyword_table[] =
{
#include"pp_keywords.inc"
};
}


TokenString
process_main(Cursor  cur, Context&  ctx)
{
  TokenString  toks;

    if(*cur == '#')
    {
      cur += 1;

      auto  s = read_directive(cur);

      Cursor  cocur(s);

      toks = process_directive(cocur,ctx);
    }


    for(;;)
    {
        if(cur.compare('\n','#'))
        {
          cur += 1;

          cur.newline();

          auto  s = read_directive(cur);

          Cursor  cocur(s);

          toks += process_directive(cocur,ctx);
        }

      else
        {
          auto  tok = read_token(cur);

            if(!tok)
            {
              break;
            }


            if(tok == TokenKind::identifier)
            {
              auto&  id = tok->string;

              bool  flag = false;

                for(auto  p: keyword_table)
                {
                    if(id == p)
                    {
                      Token::change_identifier_to_keyword(tok);

                      toks.emplace_back(std::move(tok));

                      flag = true;

                      break;
                    }
                }


                if(!flag)
                {
                  toks += process_identifier(std::move(tok),cur,ctx);
                }
            }

          else
            {
              toks.emplace_back(std::move(tok));
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
      toks = preprocessing::process_main(Cursor(s),ctx);
    }


    catch(Error&  e)
    {
      e.cursor.print();

      printf("%s\n",e.what());
    }


  toks.print();
  ctx.print();
  

  return 0;
}



