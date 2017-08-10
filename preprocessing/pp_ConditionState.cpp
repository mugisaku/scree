#include"pp_ConditionState.hpp"
#include"pp.hpp"


namespace preprocessing{




ConditionState::
ConditionState()
{
  frames.emplace_back();
}




void
ConditionState::
enter()
{
  frames.back().closed = true;

  frames.emplace_back();
}


void
ConditionState::
skip()
{
  frames.emplace_back(true);
}


namespace{
bool
test_identifier(char const*  expression_text, Context const&  ctx)
{
  Cursor  cur(expression_text);

    if(!isident0(*cur))
    {
      throw Error(cur,"識別子がない");
    }


  auto  id = read_identifier(cur);

  skip_spaces(cur);

    if(*cur)
    {
      throw Error(cur,"識別子の後に余計なものがあります");
    }


  return ctx.find_macro(id);
}


bool
test_expression(char const*  expression_text, Context const&  ctx)
{
  return make_expression(expression_text,ctx).evaluate(ctx);
}


}


void
ConditionState::
check(IfDirectiveKind  k, Context const&  ctx, char const*  expression_text)
{
    if((last_if == IfDirectiveKind::else_) &&
       (k       == IfDirectiveKind::elif ))
    {
      throw Error(Cursor(),"elseの後にelifが現れた");
    }


  last_if = k;

    switch(k)
    {
  case(IfDirectiveKind::if_):
        if(!is_locked() && test_expression(expression_text,ctx)){enter();}
      else                                                      { skip();}
      break;
  case(IfDirectiveKind::ifdef):
        if(!is_locked() && test_identifier(expression_text,ctx)){enter();}
      else                                                      { skip();}
      break;
  case(IfDirectiveKind::ifndef):
        if(!is_locked() && !test_identifier(expression_text,ctx)){enter();}
      else                                                       { skip();}
      break;
  case(IfDirectiveKind::elif):
      frames.pop_back();

        if(!is_locked() && is_not_closed() && test_expression(expression_text,ctx)){enter();}
      else                                                                         { skip();}
      break;
  case(IfDirectiveKind::else_):
      frames.pop_back();

        if(!is_locked() && is_not_closed()){enter();}
      else                                 { skip();}
      break;
  case(IfDirectiveKind::endif):
      frames.pop_back();

      frames.back().closed = false;
      break;
    }
}


}




