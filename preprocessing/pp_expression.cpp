#include"pp_expression.hpp"
#include"pp_context.hpp"




namespace preprocessing{




Expression::
Expression(Mnemonic  mn, Expression*  l, Expression*  r, Expression*  c):
kind(l? ExpressionKind::operation:ExpressionKind::operator_),
mnemonic(mn),
left(l),
right(r),
condition(c)
{
}




Expression&
Expression::
operator=(Expression const&  rhs) noexcept
{
  clear();

  kind = rhs.kind;
  mnemonic = rhs.mnemonic;
  token = rhs.token;
  condition = rhs.condition? new Expression(*rhs.condition):nullptr;
  left      = rhs.left     ? new Expression(*rhs.left     ):nullptr;
  right     = rhs.right    ? new Expression(*rhs.right    ):nullptr;

  return *this;
}


Expression&
Expression::
operator=(Expression&&  rhs) noexcept
{
  clear();

  std::swap(kind,rhs.kind);

  mnemonic = rhs.mnemonic;

  token = std::move(rhs.token);

  std::swap(condition,rhs.condition);
  std::swap(left     ,rhs.left     );
  std::swap(right    ,rhs.right    );

  return *this;
}




void
Expression::
clear()
{
  delete left          ;
         left = nullptr;

  delete right          ;
         right = nullptr;

  delete condition          ;
         condition = nullptr;

      kind = ExpressionKind::null;
  mnemonic =        Mnemonic::nop;
}


long
Expression::
operate(Context const&  ctx) const
{
    if(condition)
    {
      return condition->evaluate(ctx)?  left->evaluate(ctx)
                                     : right->evaluate(ctx);
    }


  auto  lv =  left->evaluate(ctx);

    if(mnemonic == Mnemonic::log_or)
    {
        if(lv)
        {
          return 1;
        }


      auto  rv = right->evaluate(ctx);

      return rv? 1:0;
    }

  else
    if(mnemonic == Mnemonic::log_and)
    {
        if(!lv)
        {
          return 0;
        }


      auto  rv = right->evaluate(ctx);

      return rv? 1:0;
    }



       if(mnemonic == Mnemonic::neg    ){return -lv;}
  else if(mnemonic == Mnemonic::log_not){return !lv;}
  else if(mnemonic == Mnemonic::bit_not){return ~lv;}


  auto  rv = right->evaluate(ctx);

    switch(mnemonic)
    {
  case(Mnemonic::add): return lv+rv;
  case(Mnemonic::sub): return lv-rv;
  case(Mnemonic::mul): return lv*rv;
  case(Mnemonic::div):
        if(!rv)
        {
          throw Error(Cursor(),"ゼロ除算\n");
        }


      return lv/rv;
      break;
  case(Mnemonic::rem):
        if(!rv)
        {
          throw Error(Cursor(),"ゼロ除算\n");
        }


      return lv%rv;
      break;
  case(Mnemonic::shl    ): return lv<<rv;
  case(Mnemonic::shr    ): return lv>>rv;
  case(Mnemonic::bit_and): return lv&rv;
  case(Mnemonic::bit_or ): return lv|rv;
  case(Mnemonic::bit_xor): return lv^rv;
  case(Mnemonic::eq     ): return (lv == rv)? 1:0;
  case(Mnemonic::neq    ): return (lv != rv)? 1:0;
  case(Mnemonic::lt     ): return (lv <  rv)? 1:0;
  case(Mnemonic::lteq   ): return (lv <= rv)? 1:0;
  case(Mnemonic::gt     ): return (lv >  rv)? 1:0;
  case(Mnemonic::gteq   ): return (lv >= rv)? 1:0;
  default:
      throw Error(Cursor(),"");
    }


  return 0;
}


bool
Expression::
is_unary_operation() const
{
  return((mnemonic == Mnemonic::neg    ) ||
         (mnemonic == Mnemonic::bit_not) ||
         (mnemonic == Mnemonic::log_not));
}


bool
Expression::
is_binary_operation() const
{
  return((mnemonic == Mnemonic::add    ) ||
         (mnemonic == Mnemonic::sub    ) ||
         (mnemonic == Mnemonic::mul    ) ||
         (mnemonic == Mnemonic::div    ) ||
         (mnemonic == Mnemonic::rem    ) ||
         (mnemonic == Mnemonic::shl    ) ||
         (mnemonic == Mnemonic::shr    ) ||
         (mnemonic == Mnemonic::bit_or ) ||
         (mnemonic == Mnemonic::bit_and) ||
         (mnemonic == Mnemonic::bit_xor) ||
         (mnemonic == Mnemonic::log_or ) ||
         (mnemonic == Mnemonic::log_and) ||
         (mnemonic == Mnemonic::eq     ) ||
         (mnemonic == Mnemonic::neq    ) ||
         (mnemonic == Mnemonic::lt     ) ||
         (mnemonic == Mnemonic::lteq   ) ||
         (mnemonic == Mnemonic::gt     ) ||
         (mnemonic == Mnemonic::gteq   ));
}


long
Expression::
evaluate(Context const&  ctx) const
{
  long  l;

    switch(kind)
    {
  case(ExpressionKind::null):
  case(ExpressionKind::undefined):
  case(ExpressionKind::operator_):
      throw Error("未定義の値");
      break;
  case(ExpressionKind::operation):
      l = operate(ctx);
      break;
  case(ExpressionKind::operand):
      l = token.to_integer();
      break;
    }


  return l;
}


void
Expression::
print() const
{
    switch(kind)
    {
  case(ExpressionKind::null):
      printf("NULL ");
      break;
  case(ExpressionKind::undefined):
      printf("UNDEFINED ");
      break;
  case(ExpressionKind::operator_):
      print_mnemonic();
      break;
  case(ExpressionKind::operation):
        if(is_unary_operation())
        {
          print_mnemonic();

          if(left){left->print();}
        }

      else
        if(condition)
        {
          printf("(");

          if(condition){condition->print();}

          printf(")?");

          if(left){left->print();}

          printf("):(");

          if(right){right->print();}

          printf(")");
        }

      else
        {
          printf("(");

          if(left){left->print();}

          print_mnemonic();

          if(right){right->print();}

          printf(")");
        }
      break;
  case(ExpressionKind::operand):
      token.print();
      break;
    }
}


void
Expression::
print_mnemonic() const
{
  char const*  s = "";

    switch(mnemonic)
    {
  case(Mnemonic::nop): s = "_";break;
  case(Mnemonic::add): s = "+";break;
  case(Mnemonic::sub): s = "-";break;
  case(Mnemonic::mul): s = "*";break;
  case(Mnemonic::div): s = "/";break;
  case(Mnemonic::rem): s = "%";break;
  case(Mnemonic::shl): s = "<<";break;
  case(Mnemonic::shr): s = ">>";break;
  case(Mnemonic::bit_and): s = "&";break;
  case(Mnemonic::bit_or): s = "|";break;
  case(Mnemonic::bit_xor): s = "^";break;
  case(Mnemonic::bit_not): s = "~";break;
  case(Mnemonic::eq): s = "==";break;
  case(Mnemonic::neq): s = "!=";break;
  case(Mnemonic::lt): s = "<";break;
  case(Mnemonic::lteq): s = "<=";break;
  case(Mnemonic::gt): s = ">";break;
  case(Mnemonic::gteq): s = ">=";break;
  case(Mnemonic::log_or): s = "||";break;
  case(Mnemonic::log_and): s = "&&";break;
  case(Mnemonic::log_not): s = "!";break;
  case(Mnemonic::neg): s = "-";break;
    }


  printf("%s",s);
}


}




