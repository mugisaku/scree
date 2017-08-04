#include"pp.hpp"
#include<cstdlib>




namespace preprocessing{




namespace{


enum class
ElementKind
{
  operand,
  add,
  sub,
  mul,
  div,
  rem,
  shl,
  shr,
  bit_and,
  bit_or,
  bit_xor,
  eq,
  neq,
  lt,
  lteq,
  gt,
  gteq,
  log_or,
  log_and,

};


struct
Element
{
  ElementKind  kind;

  long  operand;

  constexpr Element(ElementKind  k): kind(                   k), operand(0){}
  constexpr Element(long         l): kind(ElementKind::operand), operand(l){}

};


int
precedence(ElementKind  e)
{
  int  p = 80;

    switch(e)
    {
  case(ElementKind::add    ): p -= 6;break;
  case(ElementKind::sub    ): p -= 6;break;
  case(ElementKind::mul    ): p -= 5;break;
  case(ElementKind::div    ): p -= 5;break;
  case(ElementKind::rem    ): p -= 5;break;
  case(ElementKind::shl    ): p -= 7;break;
  case(ElementKind::shr    ): p -= 7;break;
  case(ElementKind::bit_and): p -= 10;break;
  case(ElementKind::bit_xor): p -= 11;break;
  case(ElementKind::bit_or ): p -= 12;break;

  case(ElementKind::eq     ): p -= 9;break;
  case(ElementKind::neq    ): p -= 9;break;
  case(ElementKind::lt     ): p -= 8;break;
  case(ElementKind::lteq   ): p -= 8;break;
  case(ElementKind::gt     ): p -= 8;break;
  case(ElementKind::gteq   ): p -= 8;break;
  case(ElementKind::log_and): p -= 13;break;
  case(ElementKind::log_or ): p -= 14;break;
    }


  return p;
}


bool
operator<(ElementKind  a, ElementKind  b)
{
  return precedence(a) < precedence(b);
}


long
operate(std::vector<char>&  stack, long  l)
{
    while(stack.size())
    {
        switch(stack.back())
        {
      case('-'): l = -l;break;
      case('~'): l = ~l;break;
      case('!'): l = !l;break;
        }


      stack.pop_back();
    }


  return l;
}


}


long
value_expression(TokenString::const_iterator&  it, Context const&  ctx)
{
  std::vector<char>          unop_stack;
  std::vector<ElementKind>  binop_stack;

  std::vector<Element>  output;

    while(*it)
    {
      auto&  tok = *it;

        if(tok == TokenKind::operator_)
        {
          it += 1;

            if(tok == "(")
            {
              auto  l = value_expression(it,ctx);

              output.emplace_back(operate(unop_stack,l));
            }

          else
            if(tok == ")")
            {
              break;
            }

          else
            if(output.empty() || (output.back().kind != ElementKind::operand))
            {
                   if(tok == "!"){unop_stack.emplace_back('!');}
              else if(tok == "~"){unop_stack.emplace_back('~');}
              else if(tok == "-"){unop_stack.emplace_back('-');}
            }

          else
            {
              ElementKind  op;

                   if(tok == "+" ){op = ElementKind::add;}
              else if(tok == "-" ){op = ElementKind::sub;}
              else if(tok == "*" ){op = ElementKind::mul;}
              else if(tok == "/" ){op = ElementKind::div;}
              else if(tok == "%" ){op = ElementKind::rem;}
              else if(tok == "|" ){op = ElementKind::bit_or;}
              else if(tok == "&" ){op = ElementKind::bit_and;}
              else if(tok == "^" ){op = ElementKind::bit_xor;}
              else if(tok == "=="){op = ElementKind::eq;}
              else if(tok == "!="){op = ElementKind::neq;}
              else if(tok == "<" ){op = ElementKind::lt;}
              else if(tok == "<="){op = ElementKind::lteq;}
              else if(tok == ">" ){op = ElementKind::gt;}
              else if(tok == ">="){op = ElementKind::gteq;}
              else if(tok == "<<"){op = ElementKind::shl;}
              else if(tok == ">>"){op = ElementKind::shr;}
              else if(tok == "&&"){op = ElementKind::log_and;}
              else if(tok == "||"){op = ElementKind::log_or;}
              else {throw Error(Cursor(),"使えない演算子");}

                while(binop_stack.size() && (op < binop_stack.back()))
                {
                  output.emplace_back(binop_stack.back());

                  binop_stack.pop_back();
                }


              binop_stack.emplace_back(op);
            }
        }

      else
        if(tok == TokenKind::binary_integer)
        {
          it += 1;

          auto  p = tok->data();

            if(*p == '0')
            {
              p += 2;
            }


          auto  l = std::strtol(p,nullptr,2);

          output.emplace_back(operate(unop_stack,l));
        }

      else
        if(tok == TokenKind::octal_integer)
        {
          it += 1;

          auto  p = tok->data();

            if(*p == '0')
            {
              p += 2;
            }


          auto  l = std::strtol(p,nullptr,8);

          l = operate(unop_stack,l);

          output.emplace_back(operate(unop_stack,l));
        }

      else
        if(tok == TokenKind::decimal_integer)
        {
          it += 1;

          auto  p = tok->data();

          auto  l = std::strtol(p,nullptr,10);

          output.emplace_back(operate(unop_stack,l));
        }

      else
        if(tok == TokenKind::hexadecimal_integer)
        {
          it += 1;

          auto  p = tok->data();

            if(*p == '0')
            {
              p += 2;
            }


          auto  l = std::strtol(p,nullptr,16);

          output.emplace_back(operate(unop_stack,l));
        }

      else
        if(tok == TokenKind::character)
        {
          it += 1;

          output.emplace_back(operate(unop_stack,tok->data()[0]));
        }

      else
        if(tok == TokenKind::identifier)
        {
          it += 1;

            if(tok == "defined")
            {
              bool  enclosed = (*it == "(");

                if(enclosed)
                {
                  it += 1;
                }


                if(*it != TokenKind::identifier)
                {
                  throw Error(Cursor(),"%s definedのあとに識別子がない",__func__);
                }


              long  l = ctx.find_macro(**it)? 1:0;

              output.emplace_back(operate(unop_stack,l));

              it += 1;

                if(enclosed)
                {
                    if(*it != ")")
                    {
                      throw Error(Cursor(),"%s definedのあとの括弧が閉じられていない",__func__);
                    }


                  it += 1;
                }
            }

          else
            {
              auto  toks = process_identifier(tok,it,ctx);

                if(toks.size())
                {
                  auto  coit = toks.cbegin();

                  auto  l = value_expression(coit,ctx);

                  l = operate(unop_stack,l);

                  output.emplace_back(operate(unop_stack,l));
                }

              else
                {
                  throw Error(Cursor(),"%s 不明な識別子 %s",__func__,tok->data());
                }
            }
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


  std::vector<long>  calc;

    for(auto&  e: output)
    {
        if(e.kind == ElementKind::operand)
        {
          calc.emplace_back(e.operand);
        }

      else
        {
            if(calc.size() < 2)
            {
              throw Error(Cursor(),"オペランドが足りない");
            }


          auto  rv = calc.back();

          calc.pop_back();

          auto&  lv = calc.back();

            switch(e.kind)
            {
          case(ElementKind::add    ): lv  += rv;break;
          case(ElementKind::sub    ): lv  -= rv;break;
          case(ElementKind::mul    ): lv  *= rv;break;
          case(ElementKind::div    ): lv  /= rv;break;
          case(ElementKind::rem    ): lv  %= rv;break;
          case(ElementKind::shl    ): lv <<= rv;break;
          case(ElementKind::shr    ): lv >>= rv;break;
          case(ElementKind::bit_and): lv  &= rv;break;
          case(ElementKind::bit_or ): lv  |= rv;break;
          case(ElementKind::bit_xor): lv  ^= rv;break;

          case(ElementKind::eq     ): lv = (lv == rv)? 1:0;break;
          case(ElementKind::neq    ): lv = (lv != rv)? 1:0;break;
          case(ElementKind::lt     ): lv = (lv <  rv)? 1:0;break;
          case(ElementKind::lteq   ): lv = (lv >= rv)? 1:0;break;
          case(ElementKind::gt     ): lv = (lv >  rv)? 1:0;break;
          case(ElementKind::gteq   ): lv = (lv >= rv)? 1:0;break;
          case(ElementKind::log_or ): lv = (lv || rv)? 1:0;break;
          case(ElementKind::log_and): lv = (lv && rv)? 1:0;break;
            }
        }
    }


    if(calc.size() != 1)
    {
      throw Error(Cursor(),"演算結果が不正 %d",calc.size());
    }


//printf("%ld\n",calc.front());
  return calc.front();
}


long
value_expression(std::string const&  s, Context const&  ctx)
{
  auto  toks = process_text(s);

  auto  it = toks.cbegin();

  return value_expression(it,ctx);
}


}




