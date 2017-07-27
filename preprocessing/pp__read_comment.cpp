#include<string>
#include<cstdlib>
#include<cstring>
#include<cctype>
#include<vector>
#include"pp.hpp"


namespace preprocessing{


void
read_blockstyle_comment(Cursor&  cur)
{
    for(;;)
    {
      auto  const c = *cur;

        if(cur.compare("*/"))
        {
          cur += 2;

          break;
        }

      else
        if(c == '\n')
        {
          cur.newline();
        }

      else
        if(c == '\0')
        {
          throw Error(cur,"ブロックスタイルコメントの途中で終端文字");
        }

      else
        {
          cur += 1;
        }
    }
}


void
read_linestyle_comment(Cursor&  cur)
{
    for(;;)
    {
      auto  const c = *cur;

        if(c == '\n')
        {
          cur.newline();

          break;
        }

      else
        if(c == '\0')
        {
          throw Error(cur,"ラインスタイルコメントの途中で終端文字");
        }


      cur += 1;
    }
}


}




