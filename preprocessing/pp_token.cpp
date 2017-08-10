#include"pp_token.hpp"
#include"pp_unicode.hpp"
#include"pp_TokenString.hpp"
#include<cstdio>
#include<cstdlib>
#include<algorithm>




namespace preprocessing{




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
      l = *p;
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
      fprintf(out," NULL ");
      break;
  case(TokenKind::binary_integer):
  case(TokenKind::octal_integer):
  case(TokenKind::decimal_integer):
  case(TokenKind::hexadecimal_integer):
  case(TokenKind::operator_):
  case(TokenKind::identifier):
      fprintf(out,"%s ",string.data());
      break;
  case(TokenKind::string):
      fprintf(out,"\"%s\" ",string.data());
      break;
  case(TokenKind::character):
      fprintf(out,"\'%s\' ",string.data());
      break;
  case(TokenKind::directive):
      fprintf(out,"#%s",string.data());
      break;
    }


  fprintf(out,"%s",suffix.data());
}




}




