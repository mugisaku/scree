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
test_identifier(std::string const&  s, Context const&  ctx)
{
  Cursor  cur(s);

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


}


void
ConditionState::
check(IfDirectiveKind  k, Context const&  ctx, char const*  expression)
{
  std::string  s(expression? expression:"");

    if(if_stack.size())
    {
        if((k               == IfDirectiveKind::elif ) &&
           (if_stack.back() == IfDirectiveKind::else_))
        {
          throw Error(Cursor(),"elseの後にelifが現れた");
        }
    }

  else
    if((k == IfDirectiveKind::else_) ||
       (k == IfDirectiveKind::elif ) ||
       (k == IfDirectiveKind::endif))
    {
      throw Error(Cursor(),"ifが無ければ、elif.else,endifは不正");
    }


  if_stack.emplace_back(k);

    switch(k)
    {
  case(IfDirectiveKind::if_):
        if(!is_locked() && value_expression(s,ctx)){enter();}
      else                                         { skip();}
      break;
  case(IfDirectiveKind::ifdef):
        if(!is_locked() && test_identifier(s,ctx)){enter();}
      else                                        { skip();}
      break;
  case(IfDirectiveKind::ifndef):
        if(!is_locked() && !test_identifier(s,ctx)){enter();}
      else                                         { skip();}
      break;
  case(IfDirectiveKind::elif):
      frames.pop_back();

        if(!is_locked() && is_not_closed() && value_expression(s,ctx)){enter();}
      else                                                            { skip();}
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




