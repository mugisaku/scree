#include<string>
#include<cstdlib>
#include<cstring>
#include<cctype>
#include<vector>
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


enum class
IfDirectiveKind
{
  if_,
  ifdef,
  ifndef,
  elif,
  else_,
  endif,

};


class
ConditionalState
{
  std::vector<IfDirectiveKind>  if_stack;

  int  if_depth=0;

  bool  locked=false;
  bool  closed=false;

public:
  operator bool() const{return !locked;}

  bool  is_locked() const{return locked;}
  bool  is_not_closed() const{return !closed;}

  void  enter()
  {
    if_depth += 1;

    locked = false;
    closed =  true;
  }

  void  up()
  {
      if(closed)
      {
        if_depth -= 1;
      }


    locked = false;
    closed = false;
  }


  void  lock(){locked = true;}

  int  get_depth() const{return if_depth;}

  void  check(IfDirectiveKind  k, Context const&  ctx, char const*  expression);

};


bool
test_identifier(std::string const&  s, Context const&  ctx)
{
  Cursor  cur(s);

    if(!isident0(*cur))
    {
      throw Error(cur,"識別子がない");
    }


  auto  id = read_identifier(cur);

  skip_spaces(cur);

    if(*cur)
    {
      throw Error(cur,"識別子の後に余計なものがあります");
    }


  return ctx.find_macro(id);
}


void
ConditionalState::
check(IfDirectiveKind  k, Context const&  ctx, char const*  expression)
{
  std::string  s(expression? expression:"");

    if(if_stack.size())
    {
        if((k               == IfDirectiveKind::elif ) &&
           (if_stack.back() == IfDirectiveKind::else_))
        {
          throw Error(Cursor(),"elseの後にelifが現れた");
        }
    }

  else
    if((k == IfDirectiveKind::else_) ||
       (k == IfDirectiveKind::elif ) ||
       (k == IfDirectiveKind::endif))
    {
      throw Error(Cursor(),"ifが無ければ、elif.else,endifは不正");
    }


  if_stack.emplace_back(k);

    switch(k)
    {
  case(IfDirectiveKind::if_):
        if(value_expression(s,ctx)){enter();}
      else                         { lock();}
      break;
  case(IfDirectiveKind::ifdef):
        if(test_identifier(s,ctx)){enter();}
      else                        { lock();}
      break;
  case(IfDirectiveKind::ifndef):
        if(!test_identifier(s,ctx)){enter();}
      else                         { lock();}
      break;
  case(IfDirectiveKind::elif):
        if(is_not_closed())
        {
            if(value_expression(s,ctx)){enter();}
          else                         { lock();}
        }

      else
        {
          lock();
        }
      break;
  case(IfDirectiveKind::else_):
        if(is_not_closed())
        {
          enter();
        }

      else
        {
          lock();
        }
      break;
  case(IfDirectiveKind::endif):
        if(!if_depth)
        {
          throw Error(Cursor(),"対応するif文が無い");
        }


      up();
      break;
    }
}


TokenString
process_directive(std::string const&  s, Context&  ctx, ConditionalState&  cond_st)
{
  TokenString  toks;

  Cursor  cur(s);

  skip_spaces(cur);

    if(isalpha(*cur) || (*cur == '_'))
    {
      auto  id = read_identifier(cur);

      skip_space(cur);

           if(id == "include"){if(cond_st){toks = read_include(cur,ctx);}}
      else if(id == "define"){if(cond_st){read_define(       cur,ctx);}}
      else if(id == "undef" ){if(cond_st){read_undef(        cur,ctx);}}
      else if(id == "ifdef" ){cond_st.check(IfDirectiveKind::ifdef ,ctx,cur.to_pointer());}
      else if(id == "ifndef"){cond_st.check(IfDirectiveKind::ifndef,ctx,cur.to_pointer());}
      else if(id == "if"    ){cond_st.check(IfDirectiveKind::if_   ,ctx,cur.to_pointer());}
      else if(id == "elif"  ){cond_st.check(IfDirectiveKind::elif  ,ctx,cur.to_pointer());}
      else if(id == "else"  ){cond_st.check(IfDirectiveKind::else_ ,ctx,cur.to_pointer());}
      else if(id == "endif" ){cond_st.check(IfDirectiveKind::endif ,ctx,cur.to_pointer());}
      else if(id == "error"){if(cond_st){throw Error(cur,"#error");}}
      else if(id == "print"){if(cond_st){read_print(cur);}}
      else{throw Error(cur,"未対応のディレクティブ");}
    }


  return std::move(toks);
}


}


TokenString
process_token_string_that_includes_directives(TokenString const&  src, Context&  ctx)
{
  TokenString  toks;

  ConditionalState  cond_st;

  auto         it = src.cbegin();
  auto  const end = src.cend();

    while(it != end)
    {
      auto&  tok = *it     ;
                    it += 1;

        if(tok == TokenKind::directive)
        {
          toks += process_directive(*tok,ctx,cond_st);
        }

      else
        if(cond_st)
        {
            if(tok == TokenKind::identifier)
            {
              auto  res = process_identifier(tok,it,ctx);

                if(res.size())
                {
                  toks += res;
                }

              else
                {
                  toks += tok;
                }
            }

          else
            {
              toks += std::move(tok);
            }
        }
    }


    if(cond_st.get_depth())
    {
      throw Error(Cursor(),"対応するif文が無い");
    }


  return std::move(toks);
}


TokenString
process_token_string(TokenString const&  src, Context const&  ctx)
{
  TokenString  toks;

  auto         it = src.cbegin();
  auto  const end = src.cend();

    while(it != end)
    {
      auto&  tok = *it     ;
                    it += 1;

        if(tok == TokenKind::identifier)
        {
          auto  res = process_identifier(tok,it,ctx);

            if(res.size())
            {
              toks += res;
            }

          else
            {
              toks += tok;
            }
        }

      else
        {
          toks += std::move(tok);
        }
    }


  return std::move(toks);
}


}




