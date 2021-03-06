#include"pp.hpp"
#include<cctype>


namespace preprocessing{


namespace{
ParameterList
read_parameter_list(Cursor&  cur)
{
  ParameterList  parls;

  bool  va = false;

    for(;;)
    {
      skip_spaces(cur);

      auto  const c = *cur;

        if(c == ')')
        {
          cur += 1;

          break;
        }

      else
        if(isident0(c))
        {
          parls.emplace_back(read_identifier(cur));

          skip_spaces(cur);

            if(*cur == ',')
            {
              cur += 1;
            }
        }

      else
        if(cur.compare('.','.','.'))
        {
          parls.emplace_back(std::string("..."));

          cur += 3;

          va = true;
        }

      else
        {
          throw Error(cur,"仮引数リストの読み込み中に不明な文字 %c(%d)",c,c);
        }
    }


  return std::move(parls);
}
}


void
read_define(Cursor&  cur, Context&  ctx)
{
  auto  id = read_identifier(cur);


  Macro  macro(std::move(id));

    if(*cur == '(')
    {
      cur += 1;

      macro.set_function_style_flag();

      macro.set_parameter_list(read_parameter_list(cur));
    }


  TokenString  toks;

    for(;;)
    {
      skip_spaces(cur);

      auto  tok = read_token(cur);

        if(!tok)
        {
          break;
        }


      toks += std::move(tok);
    }


  macro.set_token_string(std::move(toks));

  auto  result = ctx.find_macro(id);

    if(result)
    {
        if(*result == macro)
        {
          throw Error(cur,"既に定義済みの識別子");
        }
    }


  ctx.append_macro(std::move(macro));
}




}




