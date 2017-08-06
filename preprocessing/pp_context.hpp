#ifndef PP_Context_HPP_INCLUDED
#define PP_Context_HPP_INCLUDED


#include"pp_macro.hpp"
#include<list>


namespace preprocessing{




class
Context
{
  std::vector<std::string>  include_directory_list;

  std::list<Macro>  macro_table;

public:
  Context();

  Macro const*  find_macro(const std::string&  name) const;

  void  append_macro(Macro&&  macro){macro_table.emplace_back(std::move(macro));}
  void  remove_macro(std::string const&  name);

  void  append_include_directory(std::string&&  path){include_directory_list.emplace_back(std::move(path));}

  std::vector<std::string> const&  get_include_directory_list() const{return include_directory_list;}

  void  print() const;

};


}




#endif




