#include"pp.hpp"


namespace preprocessing{


namespace{


void
read_undef(Cursor&  cur, Context&  ctx)
{
  ctx.remove_macro(read_identifier(cur));
}


void
read_print(Cursor&  cur)
{
  printf("[PRINT] ");

    for(;;)
    {
      auto  const c = *cur;

        if(isprint(c))
        {
          printf("%c",c);

          cur += 1;
        }

      else
        {
          break;
        }
    }


  printf("\n");
}


}


TokenString
process_directive(Cursor  cur, Context&  ctx)
{
  TokenString  ls;

  skip_spaces(cur);

  auto  const flag = !ctx.test_locked_flag();

    if(isalpha(*cur) || (*cur == '_'))
    {
      auto  id = read_identifier(cur);

      skip_space(cur);

           if(id == "include"){if(flag){ls = read_include(cur,ctx);}}
      else if(id == "define"){if(flag){read_define(cur,ctx);}}
      else if(id == "undef" ){if(flag){read_undef(cur,ctx);}}
      else if(id == "ifdef" ){ctx.accept_if_directive(IfDirectiveKind::ifdef,cur.to_pointer());}
      else if(id == "ifndef"){ctx.accept_if_directive(IfDirectiveKind::ifndef,cur.to_pointer());}
      else if(id == "if"    ){ctx.accept_if_directive(IfDirectiveKind::if_,cur.to_pointer());}
      else if(id == "elif"  ){ctx.accept_if_directive(IfDirectiveKind::elif,cur.to_pointer());}
      else if(id == "else"  ){ctx.accept_if_directive(IfDirectiveKind::else_,cur.to_pointer());}
      else if(id == "endif" ){ctx.accept_if_directive(IfDirectiveKind::endif,cur.to_pointer());}
      else if(id == "error"){if(flag){throw Error(cur,"#error");}}
      else if(id == "print"){if(flag){read_print(cur);}}
      else{throw Error(cur,"未対応のディレクティブ");}
    }


  return std::move(ls);
}


std::string
read_directive(Cursor&  cur)
{
  std::string  s;

    for(;;)
    {
      auto  const c = *cur;

        if(c == '\n')
        {
          break;
        }

      else
        if((c ==  ' ') ||
           (c == '\t') ||
           (c == '\r'))
        {
          cur += 1;

          s.push_back(' ');
        }

      else
        if(cur.compare("\\\n"))
        {
          cur += 1;

          cur.newline();
        }

      else
        if(iscntrl(c))
        {
          throw Error(cur,"ディレクティブの途中で制御文字");
        }

      else
        {
          cur += 1;

          s.push_back(c);
        }
    }


  return std::move(s);
}




}




