#ifndef PP_Macro_HPP_INCLUDED
#define PP_Macro_HPP_INCLUDED


#include<algorithm>
#include<string>
#include<vector>
#include<list>


namespace preprocessing{


using ArgumentList = std::vector<std::string>;


struct Context;


class
Macro
{
  std::string  name;

  std::vector<std::string>  parameter_list;

  bool      function_style_flag;
  bool  variable_arguments_flag;

  std::string  text;

public:
  Macro(std::string&&  name_, std::string&&  text_):
  name(std::move(name_)),
  text(std::move(text_)){}

  bool  operator==(std::string const&  name_) const{return name == name_;}

  bool  is_function_style() const{return function_style_flag;}

  std::string const&  get_text() const{return text;}

  int  get_value(Context const&  ctx) const;

};


}




#endif




