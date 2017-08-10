#include"pp_macro.hpp"
#include"pp.hpp"
#include<cctype>




namespace preprocessing{


namespace{


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


}


TokenString
Macro::
expand(Context const&  ctx, ArgumentList const*  args) const
{
  enum class Operator{
    none,
    stringize,
    concatenate,
  } op = Operator::none;


  Token const*  tmp = nullptr;

  TokenString  output;

  auto         it = token_string.begin();
  auto  const end = token_string.end();

    while(it != end)
    {
      auto&  tok = *it;

        if(!tok)
        {
          break;
        }

      else
        if(compare(it,'#','#'))
        {
          it += 2;

            if(!tmp)
            {
              throw Error(Cursor(),"連結演算の左辺がない");
            }


            if(op != Operator::none)
            {
              throw Error(Cursor(),"演算異常１");
            }


          op = Operator::concatenate;
        }

      else
        if(compare(it,'#'))
        {
          it += 1;

            if(op != Operator::none)
            {
              throw Error(Cursor(),"演算異常２");
            }


          op = Operator::stringize;
        }

      else
        if(tok == TokenKind::directive)
        {
          throw Error(Cursor(),"ここにディレクティブは現れないはず");
        }

      else
        {
          it += 1;

            if(!tmp)
            {
                if(op == Operator::stringize)
                {
                  output += Token(TokenKind::string,std::string(*tok));
                }

              else
                {
                  tmp = &tok;
                }
            }

          else
            {
                if(op == Operator::stringize)
                {
                  output += Token(TokenKind::string,std::string(**tmp));
                  output += Token(TokenKind::string,std::string( *tok));

                  tmp = nullptr;
                }

              else
                if(op == Operator::concatenate)
                {
                  auto  s = **tmp+*tok;

                  auto  toks = tokenize_sub_text(s.data());

                  process_token_string(toks,ctx);

                  output += std::move(toks);

                  tmp = nullptr;
                }

              else
                {
                    if(*tmp == TokenKind::identifier)
                    {
                      process_identifier(--it,output,ctx,this,args);
                    }

                  else
                    {
                      output += *tmp;
                    }


                  tmp = &tok;
                }


              op = Operator::none;
            }
        }
    }


    if(tmp)
    {
      output += *tmp;
    }


  return std::move(output);
}


}




