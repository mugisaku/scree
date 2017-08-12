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
process_directive(char const*  s, Context&  ctx, ConditionState&  cond_st)
{
  TokenString  toks;

  Cursor  cur(s);

  skip_spaces(cur);

    if(isalpha(*cur) || (*cur == '_'))
    {
      Cursor  const saved = cur;

      auto  id = read_identifier(cur);

      skip_space(cur);

        try
        {
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
          else if(id == "pragma"){if(cond_st){}}
          else if(id == "warning"){if(cond_st){}}
          else if(id == "print"){if(cond_st){read_print(cur);}}
          else{throw Error(cur,"未対応のディレクティブ %s",id.data());}
        }


        catch(Error&  e)
        {
          saved.print();

          throw;
        }
    }


  return std::move(toks);
}


void
step_process_token_string_that_includes_directives(TokenString&  toks, Context&  ctx)
{
  TokenString  tmps;

  ConditionState  cond_st;

  TokenKind  last_k = TokenKind::null;

  auto         it = toks.begin();
  auto  const end = toks.end();

    while(it != end)
    {
      auto&  tok = *it     ;
                    it += 1;

        if(tok == TokenKind::directive)
        {
          tmps += process_directive(tok->data(),ctx,cond_st);
        }

      else
        if(cond_st)
        {
            if(tok == TokenKind::identifier)
            {
                if(Token::is_integer(last_k) && is_integer_suffix(*tok))
                {
                  tmps.back().set_suffix(*tok);
                }

              else
                if(process_identifier(--const_cast<TokenString::const_iterator&>(it),tmps,ctx))
                {
                }
            }

          else
            if((tok == TokenKind::character) && (last_k == TokenKind::identifier))
            {
              auto&  bk = tmps.back();

                if((bk == "L" ) ||
                   (bk == "u8") ||
                   (bk == "u" ) ||
                   (bk == "U"))
                {
                  tok.set_prefix(*bk);

                  bk = std::move(tok);
                }

              else
                {
                  throw Error("文字定数に不明な接頭辞　%s",bk->data());
                }
            }

          else
            {
              tmps += std::move(tok);
            }
        }


      last_k = tmps.back().get_kind();
    }


    if(cond_st.get_depth() > 1)
    {
//      throw Error(Cursor(),"対応するif文が無い %d",cond_st.get_depth());
    }


  toks = std::move(tmps);
}


bool
step_process_token_string_for_expression(TokenString&  toks, Context const&  ctx)
{
  bool  modified = false;

  TokenString  tmps;

  TokenKind  last_k = TokenKind::null;

  auto         it = toks.begin();
  auto  const end = toks.end();

    while(it != end)
    {
      auto&  tok = *it     ;
                    it += 1;

        if(tok == TokenKind::identifier)
        {
            if(tok == "defined")
            {
              bool  const enclosed = (*it == "(");

                if(enclosed)
                {
                  it += 1;
                }


                if(*it != TokenKind::identifier)
                {
                  throw Error(Cursor(),"%s definedのあとに識別子がない",__func__);
                }


              tmps += Token(ctx.find_macro(**it)? 1:0);

              it += 1;

                if(enclosed)
                {
                    if(*it != ")")
                    {
                      throw Error(Cursor(),"%s definedのあとの括弧が閉じられていない",__func__);
                    }


                  it += 1;
                }


              modified = true;
            }

          else
            if(Token::is_integer(last_k) && is_integer_suffix(*tok))
            {
              tmps.back().set_suffix(*tok);

              modified = true;
            }

          else
            if(process_identifier(--const_cast<TokenString::const_iterator&>(it),tmps,ctx))
            {
              modified = true;
            }
        }

      else
        if((tok == TokenKind::character) && (last_k == TokenKind::identifier))
        {
          auto&  bk = tmps.back();

            if((bk == "L" ) ||
               (bk == "u8") ||
               (bk == "u" ) ||
               (bk == "U"))
            {
              tok.set_prefix(*bk);

              bk = std::move(tok);
            }

          else
            {
              throw Error("文字定数に不明な接頭辞　%s",bk->data());
            }


          modified = true;
        }

      else
        {
          tmps += std::move(tok);
        }


      last_k = tmps.back().get_kind();
    }


  toks = std::move(tmps);

  return modified;
}


bool
step_process_token_string(TokenString&  toks, Context const&  ctx)
{
  bool  modified = false;

  TokenString  tmps;

  TokenKind  last_k = TokenKind::null;

  auto         it = toks.begin();
  auto  const end = toks.end();

    while(it != end)
    {
      auto&  tok = *it     ;
                    it += 1;

        if(tok == TokenKind::identifier)
        {
            if(Token::is_integer(last_k) && is_integer_suffix(*tok))
            {
              tmps.back().set_suffix(*tok);

              modified = true;
            }

          else
            if(process_identifier(--const_cast<TokenString::const_iterator&>(it),tmps,ctx))
            {
              modified = true;
            }
        }

      else
        if((tok == TokenKind::character) && (last_k == TokenKind::identifier))
        {
          auto&  bk = tmps.back();

            if((bk == "L" ) ||
               (bk == "u8") ||
               (bk == "u" ) ||
               (bk == "U"))
            {
              tok.set_prefix(*bk);

              bk = std::move(tok);

              modified = true;
            }

          else
            {
              throw Error("文字定数に不明な接頭辞　%s",bk->data());
            }
        }

      else
        {
          tmps += std::move(tok);
        }


      last_k = tmps.back().get_kind();
    }


  toks = std::move(tmps);

  return modified;
}


void
check(int&  n)
{
    if(n++ > 100)
    {
      throw Error("繰り返し処理回数が多すぎる");
    }
}


}


void
process_token_string_that_includes_directives(TokenString&  toks, Context&  ctx)
{
  step_process_token_string_that_includes_directives(toks,ctx);

  int  n = 0;

    for(;;)
    {
        if(!step_process_token_string(toks,ctx))
        {
          break;
        }


      check(n);
    }
}


void
process_token_string_for_expression(TokenString&  toks, Context const&  ctx)
{
  int  n = 0;

    for(;;)
    {
        if(!step_process_token_string_for_expression(toks,ctx))
        {
          break;
        }


      check(n);
    }
}


void
process_token_string(TokenString&  toks, Context const&  ctx)
{
  int  n = 0;

    for(;;)
    {
        if(!step_process_token_string(toks,ctx))
        {
          break;
        }


      check(n);
    }
}


}




