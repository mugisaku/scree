#include"pp_macro.hpp"




namespace preprocessing{




std::string
Macro::
replace_text(std::string const&  text, Context const&  ctx) const
{
  std::string  s;

  return std::move(s);
}


int
Macro::
get_value(Context const&  ctx) const
{
  return 0;
}


/*
std::string
Macro::
operator()(const ArgumentList&  argls) const
{
    if(!variable_arguments_flag && (parameter_list.size() != argls.size()))
    {
      throw "引数の数が一致しません";
    }


  std::string  s;

  return std::move(s);
}
*/



}




