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
  else if(cur.compare('='        )){s = "=";}
  else if(cur.compare('!'        )){s = "!";}
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


/*
void
scan_ucn(char*  buf, int  n, Cursor&  cur)
{
    while(n--)
    {
      auto  c = *cur;

        if(isxdigit(c))
        {
          cur += 1;

          *buf++ = c;
        }

      else
        {
          throw Error(cur,"%s UCNには使えない文字",__func__);
        }
    }


  *buf = 0;
}
*/


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

//          char  ubuf[16];

            switch(*cur)
            {
          case('0'):  s.push_back('\0');break;
          case('t'):  s.push_back('\t');break;
          case('r'):  s.push_back('\r');break;
          case('n'):  s.push_back('\n');break;
          case('\\'): s.push_back('\\');break;
          case('\''): s.push_back('\'');break;
          case('\"'): s.push_back('\"');break;
//          case('u'): scan_ucn(ubuf,4,cur);  s.append(ubuf);break;
//          case('U'): scan_ucn(ubuf,8,cur);  s.append(ubuf);break;
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




