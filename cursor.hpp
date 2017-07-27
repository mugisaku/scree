#ifndef Cursor_HPP_INCLUDED
#define Cursor_HPP_INCLUDED


#include<string>
#include<cstring>
#include<cstdio>
#include<exception>


class
Cursor
{
  char const*     head=nullptr;
  char const*  pointer=nullptr;

  int  line_count=0;

public:
  Cursor(std::string const&  s):
  head(s.data()), pointer(s.data()), line_count(0){}

  constexpr Cursor(){}

  constexpr Cursor(char const*  h, char const*  p, int  line_count_):
  head(h), pointer(p), line_count(line_count_){}


  constexpr Cursor  operator+(int  n) const
  {
    return Cursor(head,pointer+n,line_count);
  }

  Cursor&  operator=(const Cursor&  rhs) noexcept
  {
    head         = rhs.head;
    pointer      = rhs.pointer;
    line_count   = rhs.line_count;

    return *this;
  }

  Cursor&  operator+=(int  n)
  {
    pointer += n;

    return *this;
  }

  constexpr char const&  operator[](int  i) const{return pointer[i];}
  constexpr char  operator*() const{return *pointer;}

  void  newline()
  {
           pointer += 1;
    head = pointer     ;

    line_count += 1;
  }


  bool  compare(const char*  s) const
  {
    auto  const len = std::strlen(s);

    return(std::strncmp(pointer,s,len) == 0);
           
  }

  bool  compare(char  c0) const{return(pointer[0] == c0);}

  bool  compare(char  c0, char  c1) const{return((pointer[0] == c0) &&
                                                 (pointer[1] == c1));}

  bool  compare(char  c0, char  c1, char  c2) const{return((pointer[0] == c0) &&
                                                           (pointer[1] == c1) &&
                                                           (pointer[2] == c2));}

  void  print() const
  {
    printf("Line:%4d\n",line_count);
    printf("%c\n",*pointer);
  }

};


struct
Error: public std::exception
{
  Cursor       const  cursor;
  char const*  const message;

  Error(Cursor const&  cur, char const*  msg=""): cursor(cur), message(msg){}

  char const*  what() noexcept{return message;}

};


#endif




