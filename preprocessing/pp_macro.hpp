#ifndef PP_Macro_HPP_INCLUDED
#define PP_Macro_HPP_INCLUDED


#include<algorithm>
#include<string>
#include<vector>
#include<list>


namespace preprocessing{


using ParameterList = std::vector<std::string>;
using  ArgumentList = ParameterList;


struct Context;


class
Macro
{
  std::string  name;

  ParameterList  parameter_list;

  bool      function_style_flag=false;
  bool  variable_arguments_flag=false;

  std::string  text;

  int  find_parameter(std::string const&  id) const;

public:
  Macro(std::string&&  name_): name(std::move(name_)){}

  bool  operator==(std::string const&  name_) const{return name == name_;}

  std::string const&  get_text() const{return text;}

  void  set_text(std::string&&  txt){text = std::move(txt);}

  void  set_parameter_list(ParameterList&&  ls);

  void   set_function_style_flag(){function_style_flag = true;}

  bool  is_function_style() const{return function_style_flag;}

  bool  test_number_of_arguments(ArgumentList const&  argls) const;

  int  get_value(Context const&  ctx) const;

  void  print() const;

};


}




#endif




