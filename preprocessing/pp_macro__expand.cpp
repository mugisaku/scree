#include"pp_macro.hpp"
#include"pp.hpp"
#include<cctype>




namespace preprocessing{


namespace{
TokenString
replace(std::string&&  id, ParameterList const&  parls, ArgumentList const&  argls)
{
    if(id == "__VA_ARGS__")
    {
      TokenString  s;

        for(int  i = parls.size();  i < argls.size();  ++i)
        {
          s += Token(TokenKind::operator_,std::string(","));
          s += argls[i];
        }


      return std::move(s);
    }

  else
    {
      int  const n = parls.size();

        for(int  i = 0;  i < n;  ++i)
        {
            if(parls[i] == id)
            {
              return TokenString(argls[i]);
            }
        }


      return TokenString(std::move(id));
    }
}


bool
compare(TokenString::const_iterator  it, char  c0)
{
  return((*it == TokenKind::operator_) && (*it == c0));
}


bool
compare(TokenString::const_iterator  it, char  c0, char  c1)
{
  return(compare(it,c0) && compare(it+1,c1));
}


/*
void
Macro::
append(TokenString&  dst, Token&&  tok, Macro const*  parent)
{
  if(tmp == TokenKind::identifier)
  {
    auto  macro = ctx.find_macro(*tok);

      if(macro && (macro != parent))
      {
          if(macro->is_function_style())
          {
            it += 1;

              if(*it != '(')
              {
                throw Error(Cursor(),"%sは関数マクロですが、実引数リストがありませsん",macro->get_name().data());
              }


            it += 1;

            auto  args = read_argument_list(it,ctx);

            dst += macro->expand(ctx,&args);
          }

        else
          {
            dst += macro->expand(ctx,nullptr);
          }
      }

    else
      {
        it += 1;

        toks.emplace_back(std::move(tmp));
      }
  }

  else
  {
    output.emplace_back(std::move(tmp));
  }
}
*/


void
concatenate(TokenString&  dst, TokenString&&  src)
{
/*
  auto  tmps = *dst.back()+*tok;

  dst.pop_back();

  Cursor  cur(tmps);

  toks.emplace_back(read_token(cur));


  auto  next = read_token(cur);

    if(next)
    {
      throw Error(cur,"連結後の字句が単一ではない");
    }
*/
}


}


TokenString
Macro::
expand(Context const&  ctx, ArgumentList const*  args) const
{
  Token           tmp;
  TokenString  output;

  auto         it = token_string.cbegin();
  auto  const end = token_string.cend();

  enum class Op{
    none,
    concatenate,
    stringize,

  } op=Op::none;


    while(it != end)
    {
      auto&  tok = *it;

        if(!tok)
        {
            if(op != Op::none)
            {
              throw Error(Cursor(),"演算異常3");
            }


          break;
        }

      else
        if(compare(it,'#','#'))
        {
            if(!tmp)
            {
              throw Error(Cursor(),"字句連結の左辺が無い");
            }


            if(op != Op::none)
            {
              throw Error(Cursor(),"演算異常1");
            }


          op = Op::concatenate;

          it += 2;
        }

      else
        if(compare(it,'#'))
        {
            if(op != Op::none)
            {
              throw Error(Cursor(),"演算異常2");
            }


          op = Op::stringize;

          it += 1;
        }

      else
        if(tok == TokenKind::directive)
        {
          throw Error(Cursor(),"ここにディレクティブは現れないはず");
        }

      else
        {
            if(tok == TokenKind::identifier)
            {
//              replace();
            }


            if(op == Op::none)
            {
                if(tmp)
                {
                }


              tmp = std::move(tok);
            }

          else
            if(op == Op::concatenate)
            {
//              concatenate(toks,);
            }

          else
            if(op == Op::stringize)
            {
//               += TokenKind::string,tok.to_string();
            }


          op = Op::none;
        }
    }


  return std::move(output);
}


}




