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
read_operator(Cursor&  cur)
{
  std::string  s;

       if(cur.compare('.','.','.')){s = "...";}
  else if(cur.compare('<','<','=')){s = "<<=";}
  else if(cur.compare('>','>','=')){s = ">>=";}
  else if(cur.compare('<','<'    )){s = "<<";}
  else if(cur.compare('<','='    )){s = "<=";}
  else if(cur.compare('<'        )){s = "<";}
  else if(cur.compare('>','>'    )){s = ">>";}
  else if(cur.compare('>','='    )){s = ">=";}
  else if(cur.compare('>'        )){s = ">";}
  else if(cur.compare('+','+'    )){s = "++";}
  else if(cur.compare('-','-'    )){s = "--";}
  else if(cur.compare('-','>'    )){s = "->";}
  else if(cur.compare('+','='    )){s = "+=";}
  else if(cur.compare('-','='    )){s = "-=";}
  else if(cur.compare('*','='    )){s = "*=";}
  else if(cur.compare('/','='    )){s = "/=";}
  else if(cur.compare('%','='    )){s = "%=";}
  else if(cur.compare('|','='    )){s = "|=";}
  else if(cur.compare('&','='    )){s = "&=";}
  else if(cur.compare('^','='    )){s = "^=";}
  else if(cur.compare('=','='    )){s = "==";}
  else if(cur.compare('!','='    )){s = "!=";}
  else if(cur.compare('|','|'    )){s = "||";}
  else if(cur.compare('&','&'    )){s = "&&";}
  else if(cur.compare(':',':'    )){s = "::";}
  else if(cur.compare(':'        )){s = ":";}
  else if(cur.compare(';'        )){s = ";";}
  else if(cur.compare('='        )){s = "=";}
  else if(cur.compare('!'        )){s = "!";}
  else if(cur.compare('?'        )){s = "?";}
  else if(cur.compare('#'        )){s = "#";}
  else if(cur.compare('~'        )){s = "~";}
  else if(cur.compare('^'        )){s = "^";}
  else if(cur.compare('+'        )){s = "+";}
  else if(cur.compare('-'        )){s = "-";}
  else if(cur.compare('*'        )){s = "*";}
  else if(cur.compare('/'        )){s = "/";}
  else if(cur.compare('%'        )){s = "%";}
  else if(cur.compare('|'        )){s = "|";}
  else if(cur.compare('&'        )){s = "&";}
  else if(cur.compare(','        )){s = ",";}
  else if(cur.compare('.'        )){s = ".";}
  else if(cur.compare('('        )){s = "(";}
  else if(cur.compare(')'        )){s = ")";}
  else if(cur.compare('['        )){s = "[";}
  else if(cur.compare(']'        )){s = "]";}
  else if(cur.compare('{'        )){s = "{";}
  else if(cur.compare('}'        )){s = "}";}
  else{throw Error(cur,"不明な演算子");}


  cur += s.size();

  return std::move(s);
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


char
read_hexadecimal(Cursor&  cur)
{
  int  n = 0;

    while(isxdigit(*cur))
    {
      n <<= 4;

      auto  const c = *cur;

      cur += 1;

        if((c >= '0') &&
           (c <= '9'))
        {
          n += c-'0';
        }

      else
        {
            switch(c)
            {
          case('a'):
          case('A'): n += 10;break;
          case('b'):
          case('B'): n += 11;break;
          case('c'):
          case('C'): n += 12;break;
          case('d'):
          case('D'): n += 13;break;
          case('e'):
          case('E'): n += 14;break;
          case('f'):
          case('F'): n += 15;break;
            }
        }
    }


  cur -= 1;

  return n;
}


std::string
read_quoted(Cursor&  cur,  char  key_char)
{
  std::string  s;

    for(;;)
    {
      auto  const c = *cur;

        if(c == '\\')
        {
          s.push_back('\\');

          cur += 1;

          s.push_back(*cur);

          cur += 1;
        }

      else
        if(c == key_char)
        {
          cur += 1;

          break;
        }

      else
        if(iscntrl(c))
        {
          throw Error(cur,"クオート列に制御文字 %d",c);
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

  TokenInfo  info(cur);

  auto  const c = *cur;

    if(!c)
    {
    }

  else
    if(c == '\'')
    {
      cur += 1;

      tok = Token(TokenKind::character,read_quoted(cur,c),std::move(info));
    }

  else
    if(c == '\"')
    {
      cur += 1;

      tok = Token(TokenKind::string,read_quoted(cur,c),std::move(info));
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

      tok = ((cc == 'b')? Token(TokenKind::binary_integer     ,read_number_literal("0b",cur,     isbinary),std::move(info)):
             (cc == 'B')? Token(TokenKind::binary_integer     ,read_number_literal("0B",cur,     isbinary),std::move(info)):
             (cc == 'o')? Token(TokenKind::octal_integer      ,read_number_literal("0o",cur,      isoctal),std::move(info)):
             (cc == 'O')? Token(TokenKind::octal_integer      ,read_number_literal("0O",cur,      isoctal),std::move(info)):
             (cc == 'x')? Token(TokenKind::hexadecimal_integer,read_number_literal("0x",cur,ishexadecimal),std::move(info)):
             (cc == 'X')? Token(TokenKind::hexadecimal_integer,read_number_literal("0X",cur,ishexadecimal),std::move(info)):
                          Token(TokenKind::decimal_integer    ,std::string("0")                           ,std::move(info)));
    }

  else
    if(isdigit(c))
    {
      tok = Token(TokenKind::decimal_integer,read_number_literal("",cur,isdecimal,0),std::move(info));
    }

  else
    if(isident0(c))
    {
      tok = Token(TokenKind::identifier,read_identifier(cur),std::move(info));
    }

  else
    if(ispunct(c))
    {
      tok = Token(TokenKind::operator_,read_operator(cur),std::move(info));
    }

  else
    {
      throw Error(cur,"処理不可の文字 %c(%d)",c,c);
    }


  return std::move(tok);
}




}




