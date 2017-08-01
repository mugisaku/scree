#include"pp_token.hpp"
#include"pp_unicode.hpp"
#include"pp_TokenString.hpp"
#include<cstdio>
#include<algorithm>




namespace preprocessing{




void
Token::
print() const
{
    switch(kind)
    {
  case(TokenKind::null):
      printf(" NULL ");
      break;
  case(TokenKind::binary_integer):
      printf(" 0b%s ",string.data());
      break;
  case(TokenKind::octal_integer):
      printf(" 0o%s ",string.data());
      break;
  case(TokenKind::hexadecimal_integer):
      printf(" 0x%s ",string.data());
      break;
  case(TokenKind::decimal_integer):
  case(TokenKind::operator_):
  case(TokenKind::identifier):
      printf(" %s ",string.data());
      break;
  case(TokenKind::string):
      printf(" \"%s\" ",string.data());
      break;
  case(TokenKind::character):
      printf(" \'%s\' ",string.data());
      break;
    }
}




}




