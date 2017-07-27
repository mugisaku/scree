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


void
join(preprocessing::TokenList&  dst, preprocessing::TokenList&&  src)
{
    for(auto&&  tok: src)
    {
      dst.emplace_back(std::move(tok));
    }
}


}


namespace preprocessing{


TokenList
process_identifier(Token&&  id, Cursor&  cur, Context&  ctx)
{
  TokenList  ls;

  auto  macro = ctx.find_macro(id->string);

    if(macro)
    {
        if(macro->is_function_style())
        {
          read_spaces_and_newline(cur);

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


TokenList
process_main(Cursor  cur, Context&  ctx)
{
  TokenList  ls;

    if(*cur == '#')
    {
      cur += 1;

      auto  s = read_directive(cur);

      Cursor  cocur(s);

      join(ls,process_directive(cocur,ctx));
    }


    for(;;)
    {
      auto  const c = *cur;

        if(!c)
        {
          break;
        }

      else
        if(cur.compare('\\','\n'))
        {
          cur += 1;

          cur.newline();
        }

      else
        if(cur.compare('\n','#'))
        {
          cur += 1;

          cur.newline();

          auto  s = read_directive(cur);

          Cursor  cocur(s);

          join(ls,process_directive(cocur,ctx));
        }

      else
        if(c == '\'')
        {
          cur += 1;

          ls.emplace_back(read_character_literal(cur));
        }

      else
        if(c == '\"')
        {
          cur += 1;

          ls.emplace_back(read_string_literal(cur));
        }

      else
        if(cur.compare('/','*'))
        {
          cur += 2;

          read_blockstyle_comment(cur);
        }

      else
        if(cur.compare('/','/'))
        {
          cur += 2;

          read_linestyle_comment(cur);
        }

      else
        if(c == '0')
        {
          cur += 1;

          ls.emplace_back(read_number_literal_that_begins_by_zero(cur));
        }

      else
        if((c >= '1') &&
           (c <= '9'))
        {
          ls.emplace_back(read_decimal_number_literal(cur));
        }

      else
        if(isalpha(c) || (c == '_'))
        {
          auto  id = read_identifier(cur);

          join(ls,process_identifier(std::move(id),cur,ctx));
        }

      else
        if((c ==  ' ') ||
           (c == '\t') ||
           (c == '\r'))
        {
          cur += 1;
        }

      else
        if(c == '\n')
        {
          cur.newline();
        }

      else
        {
printf("%c\n",c);
          throw Error(cur,"処理不可の文字");
        }
    }


  return std::move(ls);
}


}


int
main(int  argc, char**  argv)
{
  auto  s = load_file(argv[1]);

  preprocessing::Context  ctx;

  preprocessing::TokenList  ls;

    try
    {
      ls = preprocessing::process_main(Cursor(s),ctx);
    }


    catch(Error&  e)
    {
      printf("%s\n",e.what());
    }


    for(auto&  tok: ls)
    {
      tok.print();

      printf("\n");
    }


  return 0;
}



