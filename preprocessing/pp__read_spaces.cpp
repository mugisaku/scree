#include"pp.hpp"


namespace preprocessing{


void
read_spaces(Cursor&  cur)
{
    for(;;)
    {
      auto  const c = *cur;

        if((c == ' ') || (c == '\t') || (c == '\r'))
        {
          cur += 1;
        }

      else
        {
          break;
        }
    }
}


void
read_spaces_and_newline(Cursor&  cur)
{
    for(;;)
    {
      auto  const c = *cur;

        if((c == ' ') || (c == '\t') || (c == '\r'))
        {
          cur += 1;
        }

      else
        if(c == '\n')
        {
          cur.newline();
        }

      else
        {
          break;
        }
    }
}


void
read_spaces_and_newline_with_escape(Cursor&  cur)
{
    for(;;)
    {
      auto  const c = *cur;

        if((c == ' ') || (c == '\t') || (c == '\r'))
        {
          cur += 1;
        }

      else
        if(c == '\n')
        {
          Error(cur,"エスケープされていない改行");
        }

      else
        if(c == '\\')
        {
          cur += 1;

            if(*cur != '\n')
            {
              Error(cur,"改行でないエスケープ");
            }


          cur.newline();
        }

      else
        {
          break;
        }
    }
}




}




