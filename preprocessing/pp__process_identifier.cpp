#include<string>
#include<cstdlib>
#include<cstring>
#include<cctype>
#include<vector>
#include"pp.hpp"
#include"pp_ConditionState.hpp"


namespace preprocessing{


namespace{
TokenString
replace(std::string const&  id, ParameterList const&  parls, ArgumentList const&  argls)
{
    if(id == "__VA_ARGS__")
    {
      TokenString  s;

        for(int  i = parls.size();  i < argls.size();  ++i)
        {
          s += Token(TokenKind::operator_,std::string(","));
          s += argls[i];
        }


      return std::move(s);
    }

  else
    {
      int  const n = parls.size();

        for(int  i = 0;  i < n;  ++i)
        {
            if(parls[i] == id)
            {
              return argls[i];
            }
        }
    }


  return TokenString();
}
}


bool
process_identifier(TokenString::const_iterator&  it, TokenString&  buf, Context const&  ctx,
                   FunctionData const*  fn)
{
  auto&  id = *it     ;
               it += 1;

    if(fn)
    {
      buf += preprocessing::replace(*id,fn->macro.get_parameter_list(),fn->argument_list);

      return true;
    }

  else
    if(*id == "__FILE__")
    {
      buf += Token(TokenKind::string,std::string(id.get_info().get_file_path()));

      return true;
    }

  else
    if(*id == "__LINE__")
    {
      buf += Token(TokenKind::decimal_integer,std::to_string(id.get_info().get_line_count()));

      return true;
    }


  auto  macro = ctx.find_macro(*id);

    if(macro && !id.test_hideset(*macro))
    {
        if(macro->is_function_style())
        {
            if(*it != '(')
            {
              throw Error(Cursor(),"%sは関数マクロですが、実引数リストがありません",macro->get_name().data());
            }


          it += 1;

          auto  coargs = read_argument_list(it,ctx);

          buf += macro->expand(ctx,&coargs);
        }

      else
        {
          buf += macro->expand(ctx,nullptr);
        }


      return true;
    }


  buf += id;

  return false;
}


}




