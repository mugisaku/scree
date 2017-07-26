#ifndef PP_Expression_HPP_INCLUDED
#define PP_Expression_HPP_INCLUDED


#include<algorithm>
#include"pp_operand.hpp"


namespace preprocessing{


struct Context;


struct
Operator
{
  char  codes[4];

  constexpr Operator(char  c0=0, char  c1=0, char  c2=0): codes{c0,c1,c2,0}{}

  constexpr uint32_t  operator*() const{return((codes[0]<<24)|(codes[1]<<16)|(codes[2]<<8));}

};


enum class
ExpressionKind
{
  null,
  operand,
  operator_,
  unary_operation,
  binary_operation,
  conditional_operation,

};


class
Expression
{
  ExpressionKind  kind=ExpressionKind::null;

  Operand   operand;
  Operator  operator_;

  Expression*   left=nullptr;
  Expression*  right=nullptr;

public:
  Expression(){}
  Expression(Operand&&  op): kind(ExpressionKind::operand), operand(std::move(op)){}
  Expression(Operator  rator, Operand&&  rand): kind(ExpressionKind::unary_operation), operator_(rator), operand(std::move(rand)){}
  Expression(Operand&&  l, Operator  rator, Operand&&  r);
  Expression(Operand&&  cond, Operand&&  l, Operand&&  r);
  Expression(const Expression&   rhs) noexcept{*this = rhs;}
  Expression(      Expression&&  rhs) noexcept{*this = std::move(rhs);}
 ~Expression(){clear();}

  Expression&  operator=(const Expression&   rhs) noexcept;
  Expression&  operator=(      Expression&&  rhs) noexcept;

  int  get_value(Context const&  ctx) const;

  void  clear();

  void  print() const;

};


}




#endif




