#include<string>
#include<cstdlib>
#include<cstring>
#include<cctype>
#include<vector>
#include"pp.hpp"


namespace preprocessing{




std::string
read_argument(Cursor&  cur)
{
  std::string  s;

    for(;;)
    {
      auto  const c = *cur;

        if((c == ',') || (c == ')'))
        {
          break;
        }

      else
        if(isprint(c))
        {
          cur += 1;

          s.push_back(c);
        }

      else
        {
          throw Error(cur,"引数の途中で処理不可の文字2");
        }
    }


  return std::move(s);
}


ArgumentList
read_argument_list(Cursor&  cur)
{
  skip_spaces_and_newline(cur);

    if(*cur == ')')
    {
      cur += 1;

      return ArgumentList();
    }


  ArgumentList  argls;

    if(isprint(*cur))
    {
      argls.emplace_back(read_argument(cur));
    }

  else
    {
      throw Error(cur,"第一引数に処理不可の文字");
    }


    for(;;)
    {
      skip_spaces_and_newline(cur);

      auto  const c = *cur;

        if(c == ')')
        {
          cur += 1;

          break;
        }

      else
        if(c == ',')
        {
          cur += 1;

          skip_spaces_and_newline(cur);

            if(isprint(*cur))
            {
              argls.emplace_back(read_argument(cur));
            }

          else
            {
              throw Error(cur,"第二引数以降に処理不可の文字");
            }
        }

      else
        {
          throw Error(cur,"第二引数以降に処理不可の文字");
        }
    }


  return std::move(argls);
}


}




