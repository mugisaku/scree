#include"pp_context.hpp"




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




