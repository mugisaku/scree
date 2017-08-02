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
read_number_literal(char const*  prefix, Cursor&  cur, IS  is, int  n=1)
{
  cur += n;

  std::string  s(prefix);

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

        if(isidentn(c))
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

          tok = ((cc == 'b')? Token(TokenKind::integer,read_number_literal("0b",cur,     isbinary)):
                 (cc == 'B')? Token(TokenKind::integer,read_number_literal("0B",cur,     isbinary)):
                 (cc == 'o')? Token(TokenKind::integer,read_number_literal("0o",cur,      isoctal)):
                 (cc == 'O')? Token(TokenKind::integer,read_number_literal("0O",cur,      isoctal)):
                 (cc == 'x')? Token(TokenKind::integer,read_number_literal("0x",cur,ishexadecimal)):
                 (cc == 'X')? Token(TokenKind::integer,read_number_literal("0X",cur,ishexadecimal)):
                              Token(TokenKind::integer,std::string("0")                          ));
        }

      else
        if(isdigit(c))
        {
          tok = Token(TokenKind::integer,read_number_literal("",cur,isdecimal,0));
        }

      else
        if(isident0(c))
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
          throw Error(cur,"処理不可の文字 %c(%d)",c,c);
        }
    }


  return std::move(tok);
}


}




