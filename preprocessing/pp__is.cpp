#include<string>
#include<cstdlib>
#include<cstring>
#include<cctype>
#include<vector>
#include"pp.hpp"


namespace preprocessing{


bool
is_u(char  c)
{
  return((c == 'u') ||
         (c == 'U'));
}


bool
is_l(char  c)
{
  return((c == 'l') ||
         (c == 'L'));
}


bool
is_ll(char const*  p)
{
  return(((p[0] == 'l') && (p[1] == 'l')) ||
         ((p[0] == 'L') && (p[1] == 'L')));
}


bool
is_ul(char const*  p)
{
  return((is_u(p[0]) && is_l(p[1])) ||
         (is_l(p[0]) && is_u(p[1])));
}


bool
is_integer_suffix(std::string const&  s)
{
  auto  p = s.data();

    switch(s.size())
    {
  case(1): return is_u( *p) || is_l( *p);
  case(2): return is_ul( p) || is_ll( p);
  case(3): return ((is_u( p[0]) && is_ll(p+1 )) ||
                   (is_ll(p   ) && is_u( p[2])));
    }


  return false;
}


}




