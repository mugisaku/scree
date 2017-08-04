#ifndef PP_Context_HPP_INCLUDED
#define PP_Context_HPP_INCLUDED


#include"pp_macro.hpp"
#include<list>


namespace preprocessing{


enum class
IfDirectiveKind
{
  if_,
  ifdef,
  ifndef,
  elif,
  else_,
  endif,

};


struct
Context
{
  static constexpr int  matched_flag = 1;
  static constexpr int   locked_flag = 2;

  std::list<Macro>  macro_table;

  std::vector<IfDirectiveKind>  if_stack;

  int  if_depth=0;

  std::vector<uint8_t>  flags_stack;

  void    set(int  flag){flags_stack.back() |=  flag;}
  void  unset(int  flag){flags_stack.back() &= ~flag;}
  bool   test(int  flag) const{return(flags_stack.back()&flag);}

public:
  Context(){flags_stack.emplace_back(0);}

  Macro const*  find_macro(const std::string&  name) const;

  void  append_macro(Macro&&  macro){macro_table.emplace_back(std::move(macro));}
  void  remove_macro(std::string const&  name);

  void  accept_if_directive(IfDirectiveKind  k, char const*  expression=nullptr);

  bool  test_locked_flag() const{return test(locked_flag);}

  void  print() const;

};


}




#endif




