#include"pp_operand.hpp"
#include"pp_expression.hpp"
#include"pp_context.hpp"
#include<cstdio>
#include<algorithm>
#include<new>




namespace preprocessing{




Operand::Operand(unsigned int  i): kind(OperandKind::integer){data.integer = i;}
Operand::Operand(std::string&&  id): kind(OperandKind::identifier){new(&data) std::string(std::move(id));}
Operand::Operand(Defined&&  de): kind(OperandKind::defined){new(&data) std::string(std::move(de.identifier));}
Operand::Operand(Expression*  expr): kind(OperandKind::expression){data.expression = expr;}




Operand&
Operand::
operator=(const Operand&  rhs) noexcept
{
  clear();

  kind = rhs.kind;

    switch(kind)
    {
  case(OperandKind::null):
      break;
  case(OperandKind::integer):
      data.integer = rhs.data.integer;
      break;
  case(OperandKind::identifier):
  case(OperandKind::defined):
      new(&data) std::string(rhs.data.identifier);
      break;
  case(OperandKind::expression):
      data.expression = new Expression(*rhs.data.expression);
      break;
    }


  return *this;
}


Operand&
Operand::
operator=(Operand&&  rhs) noexcept
{
  clear();

  std::swap(kind,rhs.kind);

    switch(kind)
    {
  case(OperandKind::null):
      break;
  case(OperandKind::integer):
      data.integer = rhs.data.integer;
      break;
  case(OperandKind::identifier):
  case(OperandKind::defined):
      new(&data) std::string(std::move(rhs.data.identifier));
      break;
  case(OperandKind::expression):
      data.expression = rhs.data.expression;
      break;
    }


  return *this;
}




void
Operand::
clear()
{
    switch(kind)
    {
  case(OperandKind::null):
  case(OperandKind::integer):
      break;
  case(OperandKind::identifier):
  case(OperandKind::defined):
      data.identifier.~basic_string();
      break;
  case(OperandKind::expression):
      delete data.expression;
      break;
    }


  kind = OperandKind::null;
}


int
Operand::
get_value(Context const&  ctx) const
{
  int  i = 0;

  Macro const*  macro;

    switch(kind)
    {
  case(OperandKind::null):
      break;
  case(OperandKind::integer):
      i = data.integer;
      break;
  case(OperandKind::identifier):
      macro = ctx.find_macro(data.identifier);

        if(macro)
        {
          i = macro->get_value(ctx);
        }
      break;
  case(OperandKind::defined):
      i = ctx.find_macro(data.identifier)? 1:0;
      break;
  case(OperandKind::expression):
      i = data.expression->get_value(ctx);
      break;
    }


  return i;
}


void
Operand::
print() const
{
    switch(kind)
    {
  case(OperandKind::null):
      printf(" NULL ");
      break;
  case(OperandKind::integer):
      printf(" %u(0x%08X)",data.integer,data.integer);
      break;
  case(OperandKind::identifier):
      printf(" %s ",data.identifier.data());
      break;
  case(OperandKind::defined):
      printf(" defined(%s) ",data.identifier.data());
      break;
  case(OperandKind::expression):
      data.expression->print();
      break;
    }
}




}




