#include<string>
#include<cstdlib>
#include<cstring>
#include<cctype>
#include<vector>
#include"pp.hpp"
#include"pp_ConditionState.hpp"


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


TokenString
process_directive(std::string const&  s, Context&  ctx, ConditionState&  cond_st)
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

  ConditionState  cond_st;

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


    if(cond_st.get_depth() > 1)
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




