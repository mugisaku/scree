#include"pp_cursor.hpp"


namespace preprocessing{




Cursor&
Cursor::
operator+=(int  n)
{
  pointer += n;

  return *this;
}


void
Cursor::
newline()
{
         pointer += 1;
  head = pointer     ;

  line_count += 1;
}


bool
Cursor::
compare(const char*  s) const
{
  auto  const len = std::strlen(s);

  return(std::strncmp(pointer,s,len) == 0);
         
}




bool
Cursor::
compare(char  c0) const
{
  return(pointer[0] == c0);
}


bool
Cursor::
compare(char  c0, char  c1) const
{
  return((pointer[0] == c0) &&
         (pointer[1] == c1));
}


bool
Cursor::
compare(char  c0, char  c1, char  c2) const
{
  return((pointer[0] == c0) &&
         (pointer[1] == c1) &&
         (pointer[2] == c2));
}




void
Cursor::
print() const
{
  printf("Line:%4d\n",line_count+1);

    if(pointer)
    {
      printf("%c\n",*pointer);
    }
}




}




