#ifndef PP_Token_HPP_INCLUDED
#define PP_Token_HPP_INCLUDED


#include<string>
#include<memory>
#include"pp_cursor.hpp"


namespace preprocessing{


struct TokenString;


enum class
TokenKind
{
  null,
  binary_integer,
  octal_integer,
  decimal_integer,
  hexadecimal_integer,
  character,
  string,
  identifier,
  operator_,
  directive,

};



class
TokenInfo
{
  std::shared_ptr<std::string>  file_path;

  int    line_count=0;
  int  column_point=0;

public:
  TokenInfo(){}
  TokenInfo(Cursor&  cur):
  file_path(cur.share_file_path()),
  line_count(cur.get_line_count()),
  column_point(cur.get_column_point()){}

  char const*  get_file_path() const{return file_path? file_path->data():"";}

  int  get_line_count() const{return line_count;}
  int  get_column_point() const{return column_point;}

};


class
Token
{
  TokenKind  kind=TokenKind::null;

  std::string  string;

  TokenInfo  info;

public:
  Token(){}
  Token(TokenKind  k, std::string&&  s, TokenInfo&&  info_=TokenInfo()):
  kind(k), string(std::move(s)), info(std::move(info_)){}

  bool  operator==(TokenKind  k) const{return kind == k;}
  bool  operator!=(TokenKind  k) const{return kind != k;}
  bool  operator==(std::string const&  s) const{return string == s;}
  bool  operator!=(std::string const&  s) const{return string != s;}
  bool  operator==(char  c) const{return(string[0] == c);}
  bool  operator!=(char  c) const{return(string[0] != c);}

  operator bool() const{return kind != TokenKind::null;}

  std::string const&  operator* () const{return  string;}
  std::string const*  operator->() const{return &string;}

  std::string  to_string() const;

  void  set_info(TokenInfo&&  info_){info = std::move(info_);}
  TokenInfo const&  get_info() const{return info;}

  std::string  release(){return std::move(string);}

  void  print() const;

};




}




#endif




