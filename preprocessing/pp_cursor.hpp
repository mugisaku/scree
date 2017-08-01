#ifndef PP_Cursor_HPP_INCLUDED
#define PP_Cursor_HPP_INCLUDED


#include<string>
#include<cstring>
#include<cstdio>
#include<exception>


namespace preprocessing{


class
Cursor
{
  char const*     head=nullptr;
  char const*  pointer=nullptr;

  int  line_count=0;

public:
  constexpr Cursor(){}

  Cursor(std::string const&  s):  head(s.data()), pointer(s.data()){}

  Cursor&  operator+=(int  n);

  char const&  operator[](int  i) const{return pointer[i];}
  char const&   operator*() const{return *pointer;}

  char const*  to_pointer() const{return pointer;};

  void  newline();

  bool  compare(const char*  s) const;

  bool  compare(char  c0                    ) const;
  bool  compare(char  c0, char  c1          ) const;
  bool  compare(char  c0, char  c1, char  c2) const;

  void  print() const;

};


struct
Error: public std::exception
{
  Cursor       const  cursor;
  char const*  const message;

  Error(Cursor const&  cur, char const*  msg=""): cursor(cur), message(msg){}

  char const*  what() noexcept{return message;}

};


}


#endif




