#include<string>
#include<cstdlib>
#include<cstring>
#include<cctype>
#include"pp.hpp"




namespace preprocessing{


namespace{


Token
read_binary_number_literal(Cursor&  cur)
{
  unsigned long long  u = 0;

  cur += 1;

    for(;;)
    {
      auto  const c = *cur;

        if((c == '0') ||
           (c == '1'))
        {
          cur += 1;

          u <<=     1;
          u  |= c-'0';
        }

      else
        {
          break;
        }
    }


  return Token(u);
}


Token
read_octal_number_literal(Cursor&  cur)
{
  unsigned long long  u = 0;

  cur += 1;

    for(;;)
    {
      auto  const c = *cur;

        if((c >= '0') &&
           (c <= '7'))
        {
          cur += 1;

          u <<=     3;
          u  |= c-'0';
        }

      else
        {
          break;
        }
    }


  return Token(u);
}


Token
read_hexadecimal_number_literal(Cursor&  cur)
{
  unsigned long long  u = 0;

  cur += 1;

    for(;;)
    {
      auto  const c = *cur;

        if((c >= '0') &&
           (c <= '9'))
        {
          cur += 1;

          u <<=     4;
          u  |= c-'0';
        }

        if((c == 'a') ||
           (c == 'A'))
        {
          cur += 1;

          u <<=   4;
          u  |= 0xA;
        }

      else
        if((c == 'b') ||
           (c == 'B'))
        {
          cur += 1;

          u <<=   4;
          u  |= 0xB;
        }

      else
        if((c == 'c') ||
           (c == 'C'))
        {
          cur += 1;

          u <<=   4;
          u  |= 0xC;
        }

      else
        if((c == 'd') ||
           (c == 'D'))
        {
          cur += 1;

          u <<=   4;
          u  |= 0xD;
        }

      else
        if((c == 'e') ||
           (c == 'E'))
        {
          cur += 1;

          u <<=   4;
          u  |= 0xE;
        }

      else
        if((c == 'f') ||
           (c == 'F'))
        {
          cur += 1;

          u <<=   4;
          u  |= 0xF;
        }

      else
        {
          break;
        }
    }


  return Token(u);
}


}


Token
read_decimal_number_literal(Cursor&  cur)
{
  unsigned long long  u = 0;

    for(;;)
    {
      auto  const c = *cur;

        if((c >= '0') &&
           (c <= '9'))
        {
          cur += 1;

          u *=    10;
          u += c-'0';
        }

      else
        {
          break;
        }
    }


  return Token(u);
}


Token
read_number_literal_that_begins_by_zero(Cursor&  cur)
{
  auto  const c = *cur;

  return(((c == 'b') || (c == 'B'))? read_binary_number_literal(     cur):
         ((c == 'o') || (c == 'O'))? read_octal_number_literal(      cur):
         ((c == 'x') || (c == 'X'))? read_hexadecimal_number_literal(cur): Token(static_cast<unsigned long long>(0)));
}


/*
Result
read(Cursor const&  cur, const char  tail_character=0)
{
  auto  const c = *cur;

  return(((c == tail_character)   )? tail_character? Result(cur+1):
                                                     Result(cur  ):
         ((c == '\0')             )? Result(Error("終了文字の前に終端文字が現れた"),cur):
         ((c == '\'')             )? read_character_literal(cur+1,String()):
         ((c == '\"')             )? read_string_literal(cur+1,String()):
         ((c == '0')              )? read_number_literal_that_begins_by_zero(cur+1):
         ((c >= '1') && (c <= '9'))? read_decimal_number_literal(cur,0):
                                     cur.compare("...")? Result(Literal(Packed('.','.','.')),cur+3):
                                     cur.compare("<<" )? Result(Literal(Packed('<','<')),cur+2):
                                     cur.compare(">>" )? Result(Literal(Packed('>','>')),cur+2):
                                     cur.compare("==" )? Result(Literal(Packed('=','=')),cur+2):
                                     cur.compare("!=" )? Result(Literal(Packed('!','=')),cur+2):
                                     cur.compare("<"  )? Result(Literal(Packed('<'    )),cur+1):
                                     cur.compare("<=" )? Result(Literal(Packed('<','=')),cur+2):
                                     cur.compare(">"  )? Result(Literal(Packed('>'    )),cur+1):
                                     cur.compare(">=" )? Result(Literal(Packed('>','=')),cur+2):
                                     cur.compare("&&" )? Result(Literal(Packed('&','&')),cur+2):
                                     cur.compare("||" )? Result(Literal(Packed('|','|')),cur+2):
                                     cur.compare("##" )? Result(Literal(Packed('#','#')),cur+2):
                                     cur.compare("+"  )? Result(Literal(Packed('+'    )),cur+1):
                                     cur.compare("-"  )? Result(Literal(Packed('-'    )),cur+1):
                                     cur.compare("*"  )? Result(Literal(Packed('*'    )),cur+1):
                                     cur.compare("/"  )? Result(Literal(Packed('/'    )),cur+1):
                                     cur.compare("%"  )? Result(Literal(Packed('%'    )),cur+1):
                                     cur.compare("|"  )? Result(Literal(Packed('|'    )),cur+1):
                                     cur.compare("&"  )? Result(Literal(Packed('&'    )),cur+1):
                                     cur.compare("^"  )? Result(Literal(Packed('^'    )),cur+1):
                                     cur.compare("!"  )? Result(Literal(Packed('!'    )),cur+1):
                                     cur.compare("~"  )? Result(Literal(Packed('~'    )),cur+1):
                                     cur.compare(","  )? Result(Literal(Packed(','    )),cur+1):
                                     cur.compare("#"  )? Result(Literal(Packed('#'    )),cur+1):
                                     cur.compare("##" )? Result(Literal(Packed('#','#')),cur+2):
                                     cur.compare(":"  )? Result(Literal(Packed(':'    )),cur+1):
                                     cur.compare("?"  )? Result(Literal(Packed('?'    )),cur+1):
                                     ((c == '(')      )? read_list_element(cur+1,')',LiteralList('(',')')): Result(Error("不明な区切り文字"),cur):
         (isalpha(c) || (c == '_'))? read_identifier(cur,String()):
                                     Result(cur));
}
*/




Token
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


  return Token(Identifier(std::move(s)));
}


Token
read_string_literal(Cursor&  cur)
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
          s.push_back(c);

          cur += 1;

          s.push_back(c);

            if(c == '\n')
            {
              cur.newline();
            }
        }

      else
        if(c == '\"')
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


  return Token(std::move(s));
}


Token
read_character_literal(Cursor&  cur)
{
/*
  std::string  s;

  auto  const qc = *cur;

  s.push_back(qc);

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
          s.push_back(c);

          cur += 1;

          s.push_back(c);

            if(c == '\n')
            {
              cur.newline();
            }
        }

      else
        if(c == qc)
        {
          s.push_back(c);

          cur += 1;

          break;
        }

      else
        {
          cur += 1;
        }
    }
*/


  return Token();
}




}



