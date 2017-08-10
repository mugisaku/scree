#ifndef PP_Cursor_HPP_INCLUDED
#define PP_Cursor_HPP_INCLUDED


#include<string>
#include<cstring>
#include<cstdio>
#include<cstdarg>
#include<exception>
#include<memory>


namespace preprocessing{


class
Cursor
{
  std::shared_ptr<std::string>  file_path;

  char const*     head=nullptr;
  char const*  pointer=nullptr;

  int  line_count=1;

public:
  constexpr Cursor(){}

  explicit Cursor(char const*  p, char const*  file_path_=nullptr):
  file_path(new std::string(file_path_? file_path_:"")),
  head(p), pointer(p){}

  Cursor&  operator+=(int  n);

  char const&  operator[](int  i) const{return pointer[i];}
  char const&   operator*() const{return *pointer;}

  int  get_line_count() const{return line_count;}
  int  get_column_point() const;

  std::shared_ptr<std::string>  share_file_path(){return file_path;}

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
  Cursor const  cursor;

  char  buffer[256];

  Error(char const*  fmt="", ...)
  {
    va_list  ap;
    va_start(ap,fmt);

    vsnprintf(buffer,sizeof(buffer),fmt,ap);

    va_end(ap);
  }

  Error(Cursor const&  cur, char const*  fmt="", ...): cursor(cur)
  {
    va_list  ap;
    va_start(ap,fmt);

    vsnprintf(buffer,sizeof(buffer),fmt,ap);

    va_end(ap);
  }

  char const*  what() noexcept{return buffer;}

};


}


#endif




