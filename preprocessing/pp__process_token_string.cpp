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


TokenString
process_directive(std::string const&  s, Context&  ctx)
{
  TokenString  ls;

  Cursor  cur(s);

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


}


TokenString
process_identifier(Token const&  id, TokenString::const_iterator&  it, Context const&  ctx, Macro const*  parent)
{
    if(*id == "__FILE__")
    {
      auto  tok = Token(TokenKind::string,std::string(id.get_info().get_file_path()));

      return TokenString(std::move(tok));
    }

  else
    if(*id == "__LINE__")
    {
      auto  tok = Token(TokenKind::decimal_integer,std::to_string(id.get_info().get_line_count()));

      return TokenString(std::move(tok));
    }


  auto  macro = ctx.find_macro(*id);

    if(macro && (macro != parent))
    {
        if(macro->is_function_style())
        {
            if(*it != '(')
            {
              throw Error(Cursor(),"%sは関数マクロですが、実引数リストがありませsん",macro->get_name().data());
            }


          it += 1;

          auto  args = read_argument_list(it,ctx);

          return macro->expand(ctx,&args);
        }

      else
        {
          return macro->expand(ctx,nullptr);
        }
    }


  return TokenString();
}


TokenString
process_token_string_that_includes_directives(TokenString const&  src, Context&  ctx)
{
  TokenString  toks;

  auto         it = src.cbegin();
  auto  const end = src.cend();

    while(it != end)
    {
      auto&  tok = *it     ;
                    it += 1;

        if(tok == TokenKind::directive)
        {
          toks += process_directive(*tok,ctx);
        }

      else
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




