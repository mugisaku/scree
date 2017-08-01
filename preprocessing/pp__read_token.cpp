#include<string>
#include<cstdlib>
#include<cstring>
#include<cctype>
#include<vector>
#include"pp.hpp"


namespace preprocessing{


namespace{


template<typename  IS>
std::string
read_number_literal(Cursor&  cur, IS  is)
{
  cur += 1;

  std::string  s;

    for(;;)
    {
      auto  const c = *cur;

        if(is(c))
        {
          cur += 1;

          s.push_back(c);
        }

      else
        {
          break;
        }
    }


  return std::move(s);
}


bool  isbinary(     char  c){return (c == '0') || (c == '1');}
bool  isoctal(      char  c){return (c >= '0') && (c <= '7');}
bool  isdecimal(    char  c){return (c >= '0') && (c <= '9');}
bool  ishexadecimal(char  c){return isdecimal(c)               ||
                                    ((c >= 'a') && (c <= 'f')) ||
                                    ((c >= 'A') && (c <= 'F'));}


}


std::string
read_identifier(Cursor&  cur)
{
  std::string  s;

    for(;;)
    {
      auto  c = *cur;

        if(isalnum(c) || (c == '_'))
        {
          cur += 1;

          s.push_back(c);
        }

      else
        {
          break;
        }
    }


  return std::move(s);
}


std::string
read_quoted(Cursor&  cur,  char  key_char)
{
  std::string  s;

    for(;;)
    {
      auto  const c = *cur;

        if(iscntrl(c))
        {
          throw Error(cur,"クオート列に制御文字");
        }

      else
        if(c == '\\')
        {
          cur += 1;

            switch(*cur)
            {
          case('0'):  s.push_back('\0');break;
          case('t'):  s.push_back('\t');break;
          case('r'):  s.push_back('\r');break;
          case('n'):  s.push_back('\n');break;
          case('\\'): s.push_back('\\');break;
          case('\''): s.push_back('\'');break;
          case('\"'): s.push_back('\"');break;
          default: throw Error(cur,"クオート列に処理不可なエスケープ文字");
            }


          cur += 1;
        }

      else
        if(c == key_char)
        {
          cur += 1;

          break;
        }

      else
        {
          cur += 1;

          s.push_back(c);
        }
    }


  return std::move(s);
}




Token
read_token(Cursor&  cur)
{
  Token  tok;

  auto  const c = *cur;

    if(c)
    {
        if(c == '\'')
        {
          cur += 1;

          tok = Token(TokenKind::character,read_quoted(cur,c));
        }

      else
        if(c == '\"')
        {
          cur += 1;

          tok = Token(TokenKind::string,read_quoted(cur,c));
        }

      else
        if(cur.compare('/','*'))
        {
          cur += 2;

          skip_blockstyle_comment(cur);
        }

      else
        if(cur.compare('/','/'))
        {
          cur += 2;

          skip_linestyle_comment(cur);
        }

      else
        if(c == '0')
        {
          cur += 1;

          auto  const cc = *cur;

          tok = (((cc == 'b') || (cc == 'B'))? Token(TokenKind::binary_integer     ,read_number_literal(cur,     isbinary)):
                 ((cc == 'o') || (cc == 'O'))? Token(TokenKind::octal_integer      ,read_number_literal(cur,      isoctal)):
                 ((cc == 'x') || (cc == 'X'))? Token(TokenKind::hexadecimal_integer,read_number_literal(cur,ishexadecimal)):
                                               Token(TokenKind::decimal_integer    ,std::string("0")                      ));
        }

      else
        if((c >= '1') &&
           (c <= '9'))
        {
          tok = Token(TokenKind::decimal_integer,read_number_literal(cur,isdecimal));
        }

      else
        if(isalpha(c) || (c == '_'))
        {
          tok = Token(TokenKind::identifier,read_identifier(cur));
        }

      else
        if(ispunct(c))
        {
          cur += 1;

          tok = Token(TokenKind::operator_,std::string({c}));
        }

      else
        {
          throw Error(cur,"処理不可の文字");
        }
    }


  return std::move(tok);
}


}




