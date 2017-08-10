#ifndef PP_Expression_HPP_INCLUDED
#define PP_Expression_HPP_INCLUDED


#include"pp_token.hpp"


namespace preprocessing{


struct Context;


enum class
ExpressionKind
{
  null,
  undefined,
  operator_,
  operation,
  operand,
 
};


enum class
Mnemonic
{
  nop,
  add,
  sub,
  mul,
  div,
  rem,
  shl,
  shr,
  bit_and,
  bit_or,
  bit_xor,
  bit_not,
  eq,
  neq,
  lt,
  lteq,
  gt,
  gteq,
  log_or,
  log_and,
  log_not,
  neg,

};


class
Expression
{
  ExpressionKind  kind=ExpressionKind::null;

  Mnemonic  mnemonic=Mnemonic::nop;

  Token  token;

  Expression*  condition=nullptr;
  Expression*       left=nullptr;
  Expression*      right=nullptr;

  long  operate(Context const&  ctx) const;

public:
  Expression(){}
  Expression(ExpressionKind  k): kind(k){}
  Expression(Mnemonic  mn, Expression*  l=nullptr, Expression*  r=nullptr, Expression*  c=nullptr);
  Expression(Token const&  tok): kind(ExpressionKind::operand), token(tok){}
  Expression(Expression const&  rhs) noexcept{*this = rhs;}
  Expression(Expression&&       rhs) noexcept{*this = std::move(rhs);}
 ~Expression(){clear();}

  Expression&  operator=(Expression const&  rhs) noexcept;
  Expression&  operator=(Expression&&       rhs) noexcept;

  bool  operator==(ExpressionKind  k) const{return kind == k;}

  Mnemonic  get_mnemonic() const{return mnemonic;}

  void  clear();

  bool   is_unary_operation() const;
  bool  is_binary_operation() const;

  long  evaluate(Context const&  ctx) const;

  void  print() const;
  void  print_mnemonic() const;

};


}




#endif




