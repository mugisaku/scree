#include<string>
#include<cstdlib>
#include<cstring>
#include<cctype>
#include<vector>
#include"pp.hpp"


namespace preprocessing{



namespace{


TokenString
read_argument(TokenString::const_iterator&  cur, Context const&  ctx)
{
  TokenString  s;

    for(;;)
    {
      auto  const c = *cur;

        if(!c)
        {
          throw Error(Cursor(),"実引数の途中で終端文字");
        }

      else
        if(c == '(')
        {
          cur += 1;

          s += c;

          s += read_argument(cur,ctx);

            while(*cur == ',')
            {
              cur += 1;

              s += *cur;

              s += read_argument(cur,ctx);
            }


          cur += 1;

          s += Token(TokenKind::operator_,std::string(")"));

          break;
        }

      else
        if((c == ')') ||
           (c == ','))
        {
          break;
        }

      else
        {
          cur += 1;

          s += c;
        }
    }


  return std::move(s);
}


}


ArgumentList
read_argument_list(TokenString::const_iterator&  cur, Context const&  ctx)
{
    if(*cur == ')')
    {
      cur += 1;

      return ArgumentList();
    }


  ArgumentList  argls;

    for(;;)
    {
      auto&  tok = *cur;

        if(!tok)
        {
          throw Error(Cursor(),"実引数リストの途中で終端文字");
        }

      else
        if(tok == ',')
        {
          throw Error(Cursor(),"実引数が無い");
        }

      else
        if(tok == ')')
        {
          cur += 1;

          break;
        }

      else
        {
          argls.emplace_back(read_argument(cur,ctx));

            if(*cur == ')')
            {
              cur += 1;

              break;
            }

          else
            if(*cur == ',')
            {
              cur += 1;
            }
        }
    }


  return std::move(argls);
}


}




