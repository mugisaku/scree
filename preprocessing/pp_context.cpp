#include"pp_context.hpp"
#include"pp_cursor.hpp"
#include"pp.hpp"




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
test_value(Cursor&  cur, Context const&  ctx)
{
    if(!*cur)
    {
      throw Error(cur,"式がない");
    }


  std::string  s;

  auto  ls = process_text(s,ctx);

  return true;
}


bool
test_identifier(Cursor&  cur, Context const&  ctx)
{
    if(!isalpha(*cur) && (*cur != '_'))
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

  Cursor  cur(s);

    switch(k)
    {
  case(IfDirectiveKind::if_):
        if(test_value(cur,*this))
        {
        }

      else
        {
          locked_flag = true;

          unlock_sc = SectionCounter(sc.maj);
        }


      minor_stack.emplace_back(sc.min);

      sc.maj = ic();
      sc.min =    0;
      break;
  case(IfDirectiveKind::ifdef):
        if(test_identifier(cur,*this))
        {
        }

      else
        {
          locked_flag = true;

          unlock_sc = SectionCounter(0,0);
        }


      minor_stack.emplace_back(sc.min);

      sc.maj = ic();
      sc.min =    0;
      break;
  case(IfDirectiveKind::ifndef):
        if(!test_identifier(cur,*this))
        {
        }

      else
        {
          locked_flag = true;

          unlock_sc = SectionCounter(0,0);
        }


      minor_stack.emplace_back(sc.min);

      sc.maj = ic();
      sc.min =    0;
      break;
  case(IfDirectiveKind::elif):
        if(test_value(cur,*this))
        {
        }

      else
        {
          locked_flag = true;

          unlock_sc = SectionCounter(0,0);
        }


      sc.min += 1;
      break;
  case(IfDirectiveKind::else_):
      sc.min += 1;
      break;
  case(IfDirectiveKind::endif):
      sc.maj -= 1;
      sc.min = minor_stack.back();

      minor_stack.pop_back();
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




