#ifndef PP_Token_HPP_INCLUDED
#define PP_Token_HPP_INCLUDED


#include<string>
#include<memory>
#include<vector>
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
  real,
  character,
  string,
  identifier,
  operator_,
  directive,

};


class
TokenInfo
{
  std::string const*  file_path;

  int    line_count=0;
  int  column_point=0;

public:
  TokenInfo(){}
  TokenInfo(Cursor&  cur):
  file_path(&cur.get_file_path()),
  line_count(cur.get_line_count()),
  column_point(cur.get_column_point()){}

  std::string const&  get_file_path() const{return *file_path;}

  int  get_line_count() const{return line_count;}
  int  get_column_point() const{return column_point;}

};


struct Macro;


class
Token
{
  TokenKind  kind=TokenKind::null;

  std::string  prefix;
  std::string  string;
  std::string  suffix;

  TokenInfo  info;

  std::vector<std::string>  hideset;

public:
  Token(){}
  Token(TokenKind  k, std::string&&  s, TokenInfo&&  info_=TokenInfo()):
  kind(k), string(std::move(s)), info(std::move(info_)){}

  Token(int  d, TokenInfo&&  info_=TokenInfo()):
  kind(TokenKind::decimal_integer),string(std::to_string(d)), info(std::move(info_)){}


  bool  operator==(TokenKind  k) const{return kind == k;}
  bool  operator!=(TokenKind  k) const{return kind != k;}
  bool  operator==(std::string const&  s) const{return string == s;}
  bool  operator!=(std::string const&  s) const{return string != s;}
  bool  operator==(char  c) const{return(string[0] == c);}
  bool  operator!=(char  c) const{return(string[0] != c);}

  std::string const&  operator* () const{return  string;}
  std::string const*  operator->() const{return &string;}

  operator bool() const{return kind != TokenKind::null;}


  static bool  is_integer(TokenKind  k);

  TokenKind  get_kind() const{return kind;}

  std::string const&  get_prefix() const{return prefix;}
  std::string const&  get_suffix() const{return suffix;}

  void  set_prefix(std::string const&  s){prefix = s;}
  void  set_suffix(std::string const&  s){suffix = s;}

  void  append_macro(Macro const&  m);

  bool  test_hideset(Macro const&  m) const;

  std::vector<std::string> const&  get_hideset() const{return hideset;}

  std::string  to_string() const;

  long  to_integer() const;

  void  set_info(TokenInfo&&  info_){info = std::move(info_);}
  TokenInfo const&  get_info() const{return info;}

  std::string  release(){return std::move(string);}

  void  print(FILE*  out=stdout) const;

};




}




#endif




