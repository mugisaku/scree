#ifndef PP_Expression_HPP_INCLUDED
#define PP_Expression_HPP_INCLUDED


#include<algorithm>
#include"pp_operand.hpp"
#include"pp_token.hpp"


namespace preprocessing{


struct Context;


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




