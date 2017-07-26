#include"pp_expression.hpp"
#include"pp_context.hpp"
#include<cstdio>
#include<cstring>




namespace preprocessing{




Expression::
Expression(Operand&&  l, Operator  rator, Operand&&  r):
kind(ExpressionKind::binary_operation), 
left(new Expression(std::move(l))),
operator_(rator),
right(new Expression(std::move(r)))
{
}


Expression::
Expression(Operand&&  cond, Operand&&  l, Operand&&  r):
kind(ExpressionKind::conditional_operation), 
operand(std::move(cond)),
left(new Expression(std::move(l))),
right(new Expression(std::move(r)))
{
}




Expression&
Expression::
operator=(const Expression&  rhs) noexcept
{
  clear();

  kind = rhs.kind;

  left  = rhs.left?  new Expression(*rhs.left ):nullptr;
  right = rhs.right? new Expression(*rhs.right):nullptr;

  operand = rhs.operand;

  operator_ = rhs.operator_;

  return *this;
}


Expression&
Expression::
operator=(Expression&&  rhs) noexcept
{
  clear();

  std::swap(left ,rhs.left );
  std::swap(right,rhs.right);

  std::swap(kind,rhs.kind);

  operand = std::move(rhs.operand);

  operator_ = rhs.operator_;

  return *this;
}



constexpr int
get(int  c0, int  c1=0, int  c2=0)
{
  return((c0<<24)|
         (c1<<16)|
         (c2<< 8));
}


int
Expression::
get_value(Context const&  ctx) const
{
  int  i = 0;

  int  rv;
  int  lv;

    switch(kind)
    {
  case(ExpressionKind::null):
      break;
  case(ExpressionKind::operand):
      i = operand.get_value(ctx);
      break;
  case(ExpressionKind::unary_operation):
      i = operand.get_value(ctx);

        switch(*operator_)
        {
      case(get('+')):        break;
      case(get('-')): i = -i;break;

      case(get('!')): i = !i;break;
      case(get('~')): i = ~i;break;
        }
      break;
  case(ExpressionKind::binary_operation):
      lv =  left->get_value(ctx);
      rv = right->get_value(ctx);

        switch(*operator_)
        {
      case(get('+')): i = (lv+rv);break;
      case(get('-')): i = (lv-rv);break;
      case(get('*')): i = (lv*rv);break;
      case(get('/')): i = (lv/rv);break;
      case(get('%')): i = (lv%rv);break;
      case(get('|')): i = (lv|rv);break;
      case(get('&')): i = (lv&rv);break;
      case(get('^')): i = (lv^rv);break;

      case(get('<','<')): i = (lv<<rv);break;
      case(get('>','>')): i = (lv>>rv);break;
      case(get('=','=')): i = (lv == rv)? 1:0;break;
      case(get('!','=')): i = (lv != rv)? 1:0;break;
      case(get('<')    ): i = (lv <  rv)? 1:0;break;
      case(get('<','=')): i = (lv <= rv)? 1:0;break;
      case(get('>')    ): i = (lv >  rv)? 1:0;break;
      case(get('>','=')): i = (lv >= rv)? 1:0;break;
      case(get('|','|')): i = (lv || rv)? 1:0;break;
      case(get('&','&')): i = (lv && rv)? 1:0;break;
        }
      break;
  case(ExpressionKind::conditional_operation):
      i = operand.get_value(ctx);
      lv =  left->get_value(ctx);
      rv = right->get_value(ctx);

      i = i? lv:rv;
      break;
    }


  return i;
}


void
Expression::
clear()
{
  operand.clear();

  delete left          ;
         left = nullptr;

  delete right          ;
         right = nullptr;

  kind = ExpressionKind::null;
}


void
Expression::
print() const
{
    switch(kind)
    {
  case(ExpressionKind::null):
      break;
  case(ExpressionKind::operand):
      operand.print();
      break;
  case(ExpressionKind::unary_operation):
      printf("%s",operator_.codes);
      operand.print();
      break;
  case(ExpressionKind::binary_operation):
      left->print();
      printf("%s",operator_.codes);
      right->print();
      break;
  case(ExpressionKind::conditional_operation):
      operand.print();
      printf("?");
      left->print();
      printf(":");
      right->print();
      break;
    }
}




}




