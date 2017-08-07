#include"pp_token.hpp"
#include"pp_unicode.hpp"
#include"pp_TokenString.hpp"
#include<cstdio>
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


void
Token::
print(FILE*  out) const
{
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
}




}




