#include<string>
#include<cstdlib>
#include<cstring>
#include<cctype>
#include<vector>
#include"pp.hpp"


namespace{




std::string
load_file(const char*  path)
{
  auto  f = fopen(path,"rb");

  std::string  s;

    if(f)
    {
        for(;;)
        {
          auto  c = fgetc(f);

            if(feof(f))
            {
              break;
            }


          s.push_back(c);
        }


      fclose(f);
    }


  return std::move(s);
}




/*
Result
read_binary_number_literal(Cursor const&  cur, int  const number_of_elements, unsigned int  const i)
{
  auto  const c = *cur;

  return((c == '0')? read_binary_number_literal(cur+1,number_of_elements+1,(i<<1)  ):
         (c == '1')? read_binary_number_literal(cur+1,number_of_elements+1,(i<<1)|1):
         !number_of_elements? Result(Error("２進数を表す要素が一つもありません"),cur):
                              Result(Literal(i),cur));
}


Result
read_octal_number_literal(Cursor const&  cur, int  const number_of_elements, unsigned int  const i)
{
  auto  const c = *cur;

  return((c >= '0') && (c <= '7')? read_octal_number_literal(cur+1,number_of_elements+1,(i<<3)|(c-'0')):
         !number_of_elements? Result(Error("8進数を表す要素が一つもありません"),cur):
                              Result(Literal(i),cur));
}


Result
read_decimal_number_literal(Cursor const&  cur, unsigned int  const i)
{
  auto  const c = *cur;

  return((c >= '0') && (c <= '9')? read_decimal_number_literal(cur+1,(i*10)+(c-'0'))
                                 : Result(Literal(i),cur));
}


Result
read_hexadecimal_number_literal(Cursor const&  cur, int  const number_of_elements, unsigned int  const i)
{
  auto  const c = *cur;

  return((c >= '0') && (c <= '9')? read_hexadecimal_number_literal(cur+1,number_of_elements+1,(i<<4)|(c-'0')):
         (c == 'a') || (c == 'A')? read_hexadecimal_number_literal(cur+1,number_of_elements+1,(i<<4)|10):
         (c == 'b') || (c == 'B')? read_hexadecimal_number_literal(cur+1,number_of_elements+1,(i<<4)|11):
         (c == 'c') || (c == 'C')? read_hexadecimal_number_literal(cur+1,number_of_elements+1,(i<<4)|12):
         (c == 'd') || (c == 'D')? read_hexadecimal_number_literal(cur+1,number_of_elements+1,(i<<4)|13):
         (c == 'e') || (c == 'E')? read_hexadecimal_number_literal(cur+1,number_of_elements+1,(i<<4)|14):
         (c == 'f') || (c == 'F')? read_hexadecimal_number_literal(cur+1,number_of_elements+1,(i<<4)|15):
         !number_of_elements? Result(Error("16進数を表す要素が一つもありません"),cur):
                              Result(Literal(i),cur));
}


Result
read_number_literal_that_begins_by_zero(Cursor const&  cur)
{
  auto  const c = *cur;

  return(((c == 'b') || (c == 'B'))? read_binary_number_literal(     cur+1,0,0):
         ((c == 'o') || (c == 'O'))? read_octal_number_literal(      cur+1,0,0):
         ((c == 'x') || (c == 'X'))? read_hexadecimal_number_literal(cur+1,0,0):
                                     Result(Literal(0),cur));
}


Result
read_character_literal(Cursor const&  cur, const String&  s)
{
  auto  const c = *cur;

  return((c == '\'' )? Result(Literal(s),cur+1):
         (iscntrl(c))? Result(Error("文字列の途中で制御文字が現れた"),cur):
                       read_character_literal(cur+1,s+c));
}


Result
check_keyword(Cursor const&  cur, String const&  s, int  const i=0)
{
  static String  const keywords[] =
  {
    String("include"),
    String("define"),
    String("undef"),
    String("if"),
    String("elif"),
    String("endif"),
    String("ifdef"),
    String("ifndef"),
    String("defined"),
    String("pragma"),
    String("line"),
    String("import"),
    String("__cplusplus"),
    String("__LINE__"),
    String("__FILE__"),
    String("__func__"),
    String("__DATE__"),
    String("__TIME__"),
    String("__STDC__"),
    String("VA_ARGS"),
  };


  constexpr int  n = sizeof(keywords)/sizeof(String);

  return((i < n)? (s == keywords[i])? Result(Literal(Keyword(s)),cur):
                                           check_keyword(cur,s,i+1):
                  Result(Literal(Identifier(s)),cur));
}




Result  read(Cursor const&  cur, const char  tail_character);


Result
read_list_element(Cursor const&  cur, const char  tail_character, const LiteralList&  ls)
{
  auto  skiped = cur.skip_spaces();

  auto  const res = read(skiped,tail_character);

  return(res.valid? (res.literal? read_list_element(res,tail_character,ls+res.literal)
                                : read_list_element(res,tail_character,ls            )):
         res.error? res:Result(Literal(ls),res));
}


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


}


namespace preprocessing{


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
          return std::move(s);
        }
    }
}


void
read_blockstyle_comment(Cursor&  cur)
{
    for(;;)
    {
      auto  const c = *cur;

        if(cur.compare("*/"))
        {
          cur += 2;

          break;
        }

      else
        if(c == '\n')
        {
          cur.newline();
        }

      else
        if(c == '\0')
        {
          throw Error(cur,"ブロックスタイルコメントの途中で終端文字");
        }

      else
        {
          cur += 1;
        }
    }
}


void
read_linestyle_comment(Cursor&  cur)
{
    for(;;)
    {
      auto  const c = *cur;

        if(c == '\n')
        {
          cur.newline();

          break;
        }

      else
        if(c == '\0')
        {
          throw Error(cur,"ラインスタイルコメントの途中で終端文字");
        }


      cur += 1;
    }
}


std::string
read_quoted(Cursor&  cur)
{
  std::string  s;

  auto  const qc = *cur;

  s.push_back(qc);

  cur += 1;

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


  return std::move(s);
}


std::string
concatenate(Cursor&  cur)
{
  std::string  s;

  read_spaces_and_newline_with_escape(cur);

    while(s.size() && (s.back() == ' '))
    {
      s.pop_back();
    }


    for(;;)
    {
      auto  const  c = *cur;

        if(iscntrl(c))
        {
          throw Error(cur,"字句連結中に制御文字");
        }

      else
        if((isalnum(c) || ispunct(c)) || (c == '#'))
        {
          s.push_back(c);

          cur += 1;
        }

      else
        {
          break;
        }
    }


  return std::move(s);
}


std::string
stringize(Cursor&  cur)
{
  std::string  s;

  read_spaces_and_newline_with_escape(cur);

  s.push_back('\"');

    for(;;)
    {
      auto  const  c = *cur;

        if(iscntrl(c))
        {
          throw Error(cur,"字句連結中に制御文字");
        }

      else
        if((isalnum(c) || ispunct(c)) || (c == '#'))
        {
          s.push_back(c);

          cur += 1;
        }

      else
        {
          s.push_back('\"');

          break;
        }
    }


  return std::move(s);
}


ArgumentList
read_argument_list(Cursor&  cur)
{
  ArgumentList  argls;

  std::string  s;

    for(;;)
    {
      read_spaces(cur);

      auto  const c = *cur;

        if(c == '\n')
        {
          cur.newline();

          break;
        }

      else
        if(c == '\0')
        {
          throw Error(cur,"引数の途中で終端文字");
        }


      cur += 1;
    }


  return std::move(argls);
}


void
push(std::string&  s, char  c, Context const&  ctx)
{
    if((c == ' ') && s.size() && (s.back() == ' '))
    {
      return;
    }


    if(ctx.get_state() == ContextState::running)
    {
      s.push_back(c);
    }
}


void
push(std::string&  s, std::string const&  t, Context const&  ctx)
{
    if(ctx.get_state() == ContextState::running)
    {
      s.append(t);
    }
}


std::string
process_main(Cursor  cur, Context&  ctx)
{
  std::string  processed;

    if(*cur == '#')
    {
      cur += 1;

      auto  s = read_directive(cur);

      Cursor  cocur(s);

      push(processed,process_directive(cocur,ctx),ctx);
    }


    for(;;)
    {
      read_spaces(cur);

      push(processed,' ',ctx);

      auto  const c = *cur;

        if(c == '\0')
        {
          break;
        }

      else
        if(cur.compare("\n#"))
        {
          cur += 2;

          auto  s = read_directive(cur);

          Cursor  cocur(s);

          push(processed,process_directive(cocur,ctx),ctx);
        }

      else
        if((c == '\'') ||
           (c == '\"'))
        {
          auto  s = read_quoted(cur);

          push(processed,s,ctx);
        }

      else
        if(c == '\\')
        {
          cur += 1;

            if(*cur == '\n')
            {
              cur.newline();
            }

          else
            {
              throw Error(cur,"不要なエスケープ");
            }
        }

      else
        if(cur.compare("##"))
        {
          cur += 2;

          auto  s = concatenate(cur);

          push(processed,s,ctx);
        }

      else
        if(c == '#')
        {
          cur += 1;

          auto  s = stringize(cur);

          push(processed,s,ctx);
        }

      else
        if(cur.compare("/*"))
        {
          cur += 2;

          read_blockstyle_comment(cur);

          push(processed,' ',ctx);
        }

      else
        if(cur.compare("//"))
        {
          cur += 2;

          read_linestyle_comment(cur);

          push(processed,' ',ctx);
        }

      else
        if(isalpha(c) || (c == '_'))
        {
          auto  text = read_identifier(cur);

          auto  macro = ctx.find_macro(text);

            if(macro)
            {
              text = process_main(macro->get_text(),ctx);
            }
            

          push(processed,text,ctx);
        }

      else
        {
          push(processed,c,ctx);

          cur += 1;
        }
    }


  return std::move(processed);
}


}


int
main(int  argc, char**  argv)
{
  auto  s = load_file(argv[1]);

  preprocessing::Context  ctx;

    try
    {
      s = preprocessing::process_main(Cursor(s),ctx);
    }


    catch(Error&  e)
    {
      printf("%s\n",e.what());
    }


  printf("%s\n",s.data());

  return 0;
}



