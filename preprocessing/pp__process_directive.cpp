#include"pp.hpp"


namespace preprocessing{


namespace{


Expression
read_expression(Cursor&  cur, Context const&  ctx)
{
}


void
read_undef(Cursor  cur, Context&  ctx)
{
  skip_spaces(cur);

  ctx.remove_macro(read_identifier(cur));
}


void
read_ifdef(Cursor  cur, Context&  ctx)
{
  skip_spaces(cur);

  auto  id = read_identifier(cur);

  ctx.push_state(ctx.find_macro(id)? ContextState::running
                                   : ContextState::looking_for_next_block);
}


void
read_ifndef(Cursor  cur, Context&  ctx)
{
  skip_spaces(cur);

  auto  id = read_identifier(cur);

  ctx.push_state(!ctx.find_macro(id)? ContextState::running
                                    : ContextState::looking_for_next_block);
}


void
read_if(Cursor  cur, Context&  ctx)
{
}


void
read_elif(Cursor  cur, Context&  ctx)
{
  auto  const st = ctx.get_state();

    if(st == ContextState::running)
    {
      ctx.change_state(ContextState::looking_for_endif);
    }

  else
    if(st == ContextState::looking_for_next_block)
    {
      ctx.change_state(ContextState::looking_for_endif);
    }

  else
    {
      throw Error(cur,"不正なelif");
    }
}


void
read_else(Cursor  cur, Context&  ctx)
{
  auto  const st = ctx.get_state();

    if(st == ContextState::running)
    {
      ctx.change_state(ContextState::looking_for_endif);
    }

  else
    if(st == ContextState::looking_for_next_block)
    {
      ctx.change_state(ContextState::running);
    }

  else
    {
      throw Error(cur,"不正なelse");
    }
}


void
read_endif(Cursor  cur, Context&  ctx)
{
  auto  const st = ctx.get_state();

    if(st == ContextState::running)
    {
        if(!ctx.pop_state())
        {
          throw Error(cur,"不正なendif");
        }
    }
}


void
read_print(Cursor  cur)
{
  skip_spaces(cur);

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
}


}


std::string
process_directive(Cursor  cur, Context&  ctx)
{
  std::string  s;

  skip_spaces(cur);

    if(isalpha(*cur) || (*cur == '_'))
    {
      auto  id = read_identifier(cur);

           if(id == "include"){s = read_include(cur,ctx);}
      else if(id == "define"){read_define(cur,ctx);}
      else if(id == "undef"){read_undef(cur,ctx);}
      else if(id == "ifdef"){read_ifdef(cur,ctx);}
      else if(id == "ifndef"){read_ifndef(cur,ctx);}
      else if(id == "if"){read_if(cur,ctx);}
      else if(id == "elif"){read_elif(cur,ctx);}
      else if(id == "else"){read_elif(cur,ctx);}
      else if(id == "endif"){read_endif(cur,ctx);}
      else if(id == "error"){throw Error(cur,"#error");}
      else if(id == "print"){read_print(cur);}
      else{throw Error(cur,"未対応のディレクティブ");}
    }


  return std::move(s);
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




