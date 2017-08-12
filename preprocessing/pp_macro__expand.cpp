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
  TokenString  output;

  auto         it = token_string.cbegin();
  auto  const end = token_string.cend();

    while(it != end)
    {
      auto&  tok = *it;
//for(auto&  s: tok.get_hideset())
{
//preprocessing::print(s);
}
        if(compare(it,'#','#'))
        {
          it += 2;

            if(!*it)
            {
              throw Error(Cursor(),"連結演算の右辺が無い");
            }


            if(output.empty())
            {
              throw Error(Cursor(),"連結演算の左辺が無い");
            }


          std::string  s;

          auto&  lhs = output.back();
          auto&  rhs = *it++;

          s +=  lhs.get_prefix();
          s += *lhs;
          s +=  lhs.get_suffix();
          s +=  rhs.get_prefix();
          s += *rhs;
          s +=  rhs.get_suffix();

          auto  tmptoks = tokenize_sub_text(s.data());


          tmptoks.append_macro(*this);

          process_token_string(tmptoks,ctx);

          output += tmptoks;
        }

      else
        if(compare(it,'#'))
        {
          it += 1;

            if(!*it)
            {
              throw Error(Cursor(),"文字列演算の項が無い");
            }


          output += Token(TokenKind::string,std::string(**it++));

          output.back().append_macro(*this);
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
                if(args)
                {
                  FunctionData  fn(*this,*args);

                  process_identifier(it,output,ctx,&fn);
                }

              else
                {
                  process_identifier(it,output,ctx,nullptr);
                }
            }

          else
            {
              it += 1;

              output += tok;
            }
        }
    }


  return std::move(output);
}


}




