#include"pp_token.hpp"
#include"pp_unicode.hpp"
#include"pp_TokenString.hpp"
#include"pp_macro.hpp"
#include<cstdio>
#include<cstdlib>
#include<algorithm>




namespace preprocessing{




void
Token::
append_macro(Macro const&  m)
{
  hideset.emplace_back(m.get_name());
}


bool
Token::
test_hideset(Macro const&  m) const
{
    for(auto&  name: hideset)
    {
        if(name == m.get_name())
        {
          return true;
        }
    }


  return false;
}


std::string
Token::
to_string() const
{
  std::string  s;

    switch(kind)
    {
  case(TokenKind::null):
      break;
  case(TokenKind::binary_integer):
  case(TokenKind::octal_integer):
  case(TokenKind::decimal_integer):
  case(TokenKind::hexadecimal_integer):
  case(TokenKind::operator_):
  case(TokenKind::identifier):
      s.append(string);
      break;
  case(TokenKind::string):
      s.push_back('\"');
      s.append(string);
      s.push_back('\"');
      break;
  case(TokenKind::character):
      s.push_back('\'');
      s.append(string);
      s.push_back('\'');
      break;
  case(TokenKind::directive):
      s.push_back('#');
      s.append(string);
      break;
    }


  return std::move(s);
}


namespace{
long
read_hexadecimal(char const*  p)
{
  long  l = 0;

  while(isxdigit(*p))
  {
    auto  const c = *p++;

    l <<= 4;

      if((c >= '0') &&
         (c <= '9'))
      {
        l |= c-'0';
      }

    else
      {
          switch(c)
          {
        case('a'):
        case('A'): l |= 10;break;
        case('b'):
        case('B'): l |= 11;break;
        case('c'):
        case('C'): l |= 12;break;
        case('d'):
        case('D'): l |= 13;break;
        case('e'):
        case('E'): l |= 14;break;
        case('f'):
        case('F'): l |= 15;break;
          }
      }
  }


  return l;
}


long
character_literal_to_integer(char const*  p)
{
    if(*p == '\\')
    {
          switch(*++p)
          {
        case('0'):  return '\0';
        case('t'):  return '\t';
        case('r'):  return '\r';
        case('n'):  return '\n';
        case('\\'): return '\\';
        case('\''): return '\'';
        case('\"'): return '\"';
        case('x'): return read_hexadecimal(p+1);
        default: throw Error("処理不可なエスケープ文字 %c",*p);
          }
    }


  return *p;
}
}


long
Token::
to_integer() const
{
  auto  p = string.data();

  long  l;

    switch(kind)
    {
  case(TokenKind::binary_integer):
        if(*p == '0')
        {
          p += 2;
        }


      l = std::strtol(p,nullptr,2);
      break;
  case(TokenKind::octal_integer):
        if(*p == '0')
        {
          p += 2;
        }


      l = std::strtol(p,nullptr,8);
      break;
  case(TokenKind::decimal_integer):
      l = std::strtol(p,nullptr,10);
      break;
  case(TokenKind::hexadecimal_integer):
        if(*p == '0')
        {
          p += 2;
        }


      l = std::strtol(p,nullptr,16);
      break;
  case(TokenKind::character):
      l = character_literal_to_integer(string.data());
      break;
  default:
      throw Error(Cursor(),"この字句は整数に変換できない\n");
    }


  return l;
}


void
Token::
print(FILE*  out) const
{
  fprintf(out,"%s",prefix.data());

    switch(kind)
    {
  case(TokenKind::null):
      fprintf(out,"NULL");
      break;
  case(TokenKind::binary_integer):
  case(TokenKind::octal_integer):
  case(TokenKind::decimal_integer):
  case(TokenKind::hexadecimal_integer):
  case(TokenKind::operator_):
  case(TokenKind::identifier):
      fprintf(out,"%s",string.data());
      break;
  case(TokenKind::string):
      fprintf(out,"\"%s\"",string.data());
      break;
  case(TokenKind::character):
      fprintf(out,"\'%s\'",string.data());
      break;
  case(TokenKind::directive):
      fprintf(out,"#%s",string.data());
      break;
    }


  fprintf(out,"%s",suffix.data());
}


bool
Token::
is_integer(TokenKind  k)
{
  return((k == TokenKind::binary_integer     ) ||
         (k == TokenKind::octal_integer      ) ||
         (k == TokenKind::decimal_integer    ) ||
         (k == TokenKind::hexadecimal_integer) ||
         (k == TokenKind::character));
}



}




