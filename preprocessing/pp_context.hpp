#ifndef PP_Context_HPP_INCLUDED
#define PP_Context_HPP_INCLUDED


#include"pp_macro.hpp"


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
  std::list<Macro>  macro_table;

  bool  locked_flag=false;

  struct IfCounter{
    int  val;

    IfCounter(int  val_=0): val(val_){}

    int  operator()(){return val += 1;}

  } ic;

  struct SectionCounter{
    int  maj;
    int  min;

    SectionCounter(int  maj_=0, int  min_=0): maj(maj_), min(min_){}

  } sc, unlock_sc;


  std::vector<int>  minor_stack;

public:
  Macro const*  find_macro(const std::string&  name) const;

  void  append_macro(Macro&&  macro){macro_table.emplace_back(std::move(macro));}
  void  remove_macro(std::string const&  name);

  void  accept_if_directive(IfDirectiveKind  k, char const*  expression=nullptr);

  bool  test_locked_flag() const{return locked_flag;}

  void  print() const;

};


}




#endif




