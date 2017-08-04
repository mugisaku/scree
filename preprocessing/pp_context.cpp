#include"pp_context.hpp"
#include"pp_cursor.hpp"
#include"pp.hpp"
#include<cstdlib>




namespace preprocessing{




void
Context::
remove_macro(std::string const&  name)
{
  auto   it = macro_table.begin();
  auto  end = macro_table.end();

    while(it != end)
    {
        if(*it == name)
        {
          macro_table.erase(it);

          break;
        }


      ++it;
    }
}


const Macro*
Context::
find_macro(const std::string&  name) const
{
    for(auto&  m: macro_table)
    {
        if(m == name)
        {
          return &m;
        }
    }


  return nullptr;
}


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


void
Context::
accept_if_directive(IfDirectiveKind  k, char const*  expression)
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

  auto&  flags = flags_stack.back();

    switch(k)
    {
  case(IfDirectiveKind::if_):
      if_depth += 1;

        if(!test(locked_flag))
        {
            if(value_expression(s,*this))
            {
              set(matched_flag);

              flags_stack.emplace_back(0);
            }

          else
            {
              set(locked_flag);
            }
        }
      break;
  case(IfDirectiveKind::ifdef):
      if_depth += 1;

        if(!test(locked_flag))
        {
            if(test_identifier(s,*this))
            {
              set(matched_flag);

              flags_stack.emplace_back(0);
            }

          else
            {
              set(locked_flag);
            }
        }
      break;
  case(IfDirectiveKind::ifndef):
      if_depth += 1;

        if(!test(locked_flag))
        {
            if(!test_identifier(s,*this))
            {
              set(matched_flag);

              flags_stack.emplace_back(0);
            }

          else
            {
              set(locked_flag);
            }
        }
      break;
  case(IfDirectiveKind::elif):
        if(!test(locked_flag))
        {
            if(!test(matched_flag))
            {
                if(value_expression(s,*this))
                {
                  set(matched_flag);

                  flags_stack.emplace_back(0);
                }

              else
                {
                  flags |= locked_flag;
                }
            }

          else
            {
              set(locked_flag);
            }
        }
      break;
  case(IfDirectiveKind::else_):
        if(!test(locked_flag))
        {
            if(!test(matched_flag))
            {
              set(matched_flag);

              flags_stack.emplace_back(0);
            }

          else
            {
              set(locked_flag);
            }
        }
      break;
  case(IfDirectiveKind::endif):
        if(!if_depth)
        {
          throw Error(Cursor(),"対応するif文が無い");
        }


      if_depth -= 1;

        if(!test(locked_flag))
        {
            if(test(matched_flag))
            {
              flags_stack.pop_back();

              unset(locked_flag);
            }
        }
      break;
    }
}


void
Context::
print() const
{
    for(auto&  m: macro_table)
    {
      m.print();

      printf("\n");
    }
}


}




