#ifndef PP_Macro_HPP_INCLUDED
#define PP_Macro_HPP_INCLUDED


#include<algorithm>
#include<string>
#include<vector>
#include"pp_TokenString.hpp"


namespace preprocessing{


using ParameterList = std::vector<std::string>;
using  ArgumentList = std::vector<TokenString>;


struct Context;


class
Macro
{
  std::string  name;

  ParameterList  parameter_list;

  bool      function_style_flag=false;
  bool  variable_arguments_flag=false;

  TokenString  token_string;

  int  find_parameter(std::string const&  id) const;

public:
  Macro(std::string&&  name_): name(std::move(name_)){}
  Macro(std::string&&  name_, int  v): name(std::move(name_)), token_string(Token(v)){}

  bool  operator==(std::string const&  name_) const{return name == name_;}

  std::string const&  get_name() const{return name;}
  TokenString const&  get_token_string() const{return token_string;}

  void  set_token_string(TokenString&&  toks){token_string = std::move(toks);}

  ParameterList const&  get_parameter_list() const{return parameter_list;}
  void                  set_parameter_list(ParameterList&&  ls);

  void   set_function_style_flag(){function_style_flag = true;}

  bool  is_function_style() const{return function_style_flag;}

  bool  test_number_of_arguments(ArgumentList const&  argls) const;

  TokenString  expand(Context const&  ctx, ArgumentList const*  args) const;

  int  get_value(Context const&  ctx) const;

  void  print() const;

};


}




#endif




