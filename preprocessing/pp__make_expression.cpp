#include"pp.hpp"
#include"pp_expression.hpp"
#include<cstdlib>




namespace preprocessing{




namespace{


enum class
ElementKind
{
  null,
  any_operator,
  operand,

};


int
precedence(Mnemonic  mn)
{
  int  p = 80;

    switch(mn)
    {
  case(Mnemonic::add    ): p -= 6;break;
  case(Mnemonic::sub    ): p -= 6;break;
  case(Mnemonic::mul    ): p -= 5;break;
  case(Mnemonic::div    ): p -= 5;break;
  case(Mnemonic::rem    ): p -= 5;break;
  case(Mnemonic::shl    ): p -= 7;break;
  case(Mnemonic::shr    ): p -= 7;break;
  case(Mnemonic::bit_and): p -= 10;break;
  case(Mnemonic::bit_xor): p -= 11;break;
  case(Mnemonic::bit_or ): p -= 12;break;

  case(Mnemonic::eq     ): p -= 9;break;
  case(Mnemonic::neq    ): p -= 9;break;
  case(Mnemonic::lt     ): p -= 8;break;
  case(Mnemonic::lteq   ): p -= 8;break;
  case(Mnemonic::gt     ): p -= 8;break;
  case(Mnemonic::gteq   ): p -= 8;break;
  case(Mnemonic::log_and): p -= 13;break;
  case(Mnemonic::log_or ): p -= 14;break;
    }


  return p;
}


bool
operator<(Mnemonic  a, Mnemonic  b)
{
  return precedence(a) < precedence(b);
}


Expression
make_unary_operation(std::vector<char>&  stack, Expression&&  expr)
{
    while(stack.size())
    {
      auto  c = stack.back();
                stack.pop_back();

      Expression  t(c == '-'? Mnemonic::neg    :
                    c == '~'? Mnemonic::bit_not:
                    c == '!'? Mnemonic::log_not:Mnemonic::nop,new Expression(std::move(expr)));

      expr = std::move(t);
    }


  return std::move(expr);
}


}


Expression
make_expression(TokenString::const_iterator&  it, Context const&  ctx)
{
  std::vector<char>       unop_stack;
  std::vector<Mnemonic>  binop_stack;

  std::vector<Expression>  output;

  auto  last_k = ElementKind::null;

    while(*it)
    {
      auto&  tok = *it;

        if(tok == TokenKind::operator_)
        {
          it += 1;

            if(tok == "(")
            {
              auto  expr = make_expression(it,ctx);

              expr = make_unary_operation(unop_stack,std::move(expr));

              output.emplace_back(std::move(expr));

              last_k = ElementKind::operand;
            }

          else
            if(tok == ")")
            {
              break;
            }

          else
            if(last_k != ElementKind::operand)
            {
                   if(tok == "!"){unop_stack.emplace_back('!');}
              else if(tok == "~"){unop_stack.emplace_back('~');}
              else if(tok == "-"){unop_stack.emplace_back('-');}

              last_k = ElementKind::any_operator;
            }

          else
            {
              Mnemonic  mn;

                   if(tok == "+" ){mn = Mnemonic::add;}
              else if(tok == "-" ){mn = Mnemonic::sub;}
              else if(tok == "*" ){mn = Mnemonic::mul;}
              else if(tok == "/" ){mn = Mnemonic::div;}
              else if(tok == "%" ){mn = Mnemonic::rem;}
              else if(tok == "|" ){mn = Mnemonic::bit_or;}
              else if(tok == "&" ){mn = Mnemonic::bit_and;}
              else if(tok == "^" ){mn = Mnemonic::bit_xor;}
              else if(tok == "=="){mn = Mnemonic::eq;}
              else if(tok == "!="){mn = Mnemonic::neq;}
              else if(tok == "<" ){mn = Mnemonic::lt;}
              else if(tok == "<="){mn = Mnemonic::lteq;}
              else if(tok == ">" ){mn = Mnemonic::gt;}
              else if(tok == ">="){mn = Mnemonic::gteq;}
              else if(tok == "<<"){mn = Mnemonic::shl;}
              else if(tok == ">>"){mn = Mnemonic::shr;}
              else if(tok == "&&"){mn = Mnemonic::log_and;}
              else if(tok == "||"){mn = Mnemonic::log_or;}
              else {throw Error(Cursor(),"使えない二項演算子 %s",tok->data());}

                while(binop_stack.size() && (mn < binop_stack.back()))
                {
                  output.emplace_back(binop_stack.back());

                  binop_stack.pop_back();
                }


              binop_stack.emplace_back(mn);

              last_k = ElementKind::any_operator;
            }
        }

      else
        if(Token::is_integer(tok.get_kind()) || (tok == TokenKind::character))
        {
          it += 1;


          auto  expr = make_unary_operation(unop_stack,Expression(tok));

          output.emplace_back(std::move(expr));

          last_k = ElementKind::operand;
        }

      else
        if(tok == TokenKind::identifier)
        {
          it += 1;


          auto  expr = make_unary_operation(unop_stack,Expression(ExpressionKind::undefined));

          output.emplace_back(std::move(expr));

          last_k = ElementKind::operand;
        }

      else
        {
          throw Error(Cursor(),"式には使えない字句");
        }
    }


    while(binop_stack.size())
    {
      output.emplace_back(binop_stack.back());

      binop_stack.pop_back();
    }


  std::vector<Expression>  calc;

    for(auto&  e: output)
    {
        if((e == ExpressionKind::undefined) ||
           (e == ExpressionKind::operand  ) ||
           (e == ExpressionKind::operation))
        {
          calc.emplace_back(std::move(e));
        }

      else
        if(e.is_binary_operation())
        {
            if(calc.size() < 2)
            {
              throw Error(Cursor(),"オペランドが足りない %d",calc.size());
            }


          auto  r = new Expression(std::move(calc.back()));

          calc.pop_back();

          auto  l = new Expression(std::move(calc.back()));

          calc.pop_back();

          calc.emplace_back(e.get_mnemonic(),l,r);
        }

      else
        {
          throw Error("%s &d",__func__,__LINE__);
        }
    }


    if(calc.size() != 1)
    {
        for(auto&  e: calc)
        {
          e.print();
      printf("\n");
        }


      printf("\n");

      throw Error(Cursor(),"演算結果が不正 %d",calc.size());
    }


  return std::move(calc.front());
}


Expression
make_expression(char const*  text, Context const&  ctx)
{
  auto  toks = tokenize_sub_text(text);
/*
printf("[[\n");
toks.print();
printf("\n--\n");
*/
  process_token_string_for_expression(toks,ctx);
/*
toks.print();
printf("\n]]\n");
*/

  auto  it = toks.cbegin();

  Expression  result;

  try{result = make_expression(it,ctx);}

    catch(Error&  e)
    {
      printf("%s\n",text);

      throw;
    }

  
  return result;
}


}




