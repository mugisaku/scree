#include"pp.hpp"
#include"pp_macro.hpp"
#include"cursor.hpp"




namespace preprocessing{


void
Macro::
set_parameter_list(ParameterList&&  ls)
{
  parameter_list = std::move(ls);

  variable_arguments_flag = (parameter_list.size() && (parameter_list.back() == "..."));

    if(variable_arguments_flag)
    {
      parameter_list.pop_back();
    }
}


int
Macro::
find_parameter(std::string const&  id) const
{
  auto  const n = parameter_list.size();

    for(int  i = 0;  i < n;  ++i)
    {
        if(parameter_list[i] == id)
        {
          return i;
        }
    }


  return -1;
}


int
Macro::
get_value(Context const&  ctx) const
{
  return 0;
}


}




