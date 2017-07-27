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

        if(c == '\\')
        {
          cur += 1;

            if(isprint(c) || (c == ' '))
            {
              cur += 1;

              s.push_back(c);
            }

          else
            {
              throw Error(cur,"引数の途中で制御文字");
            }
        }

      else
        if((c == ',') || (c == ')'))
        {
          break;
        }

      else
        if(isprint(c) || (c == ' '))
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
  ArgumentList  argls;

    for(;;)
    {
      read_spaces_and_newline(cur);

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
        }

      else
        if(isprint(c) || (c == ' '))
        {
          argls.emplace_back(read_argument(cur));
        }

      else
        {
          throw Error(cur,"引数の途中で処理不可の文字1");
        }
    }


  return std::move(argls);
}


}




