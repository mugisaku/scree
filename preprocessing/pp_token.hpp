#ifndef PP_Token_HPP_INCLUDED
#define PP_Token_HPP_INCLUDED


#include<string>


namespace preprocessing{


struct TokenString;


enum class
TokenKind
{
  null,
  integer,
  character,
  string,
  identifier,
  operator_,
  directive,

};



class
Token
{
  TokenKind  kind=TokenKind::null;

  std::string  string;

public:
  Token(){}
  Token(TokenKind  k, std::string&&  s): kind(k), string(std::move(s)){}

  bool  operator==(TokenKind  k) const{return kind == k;}
  bool  operator==(std::string const&  s) const{return string == s;}
  bool  operator==(char  c) const{return(string[0] == c);}
  bool  operator!=(char  c) const{return(string[0] != c);}

  operator bool() const{return kind != TokenKind::null;}

  std::string const&  operator* () const{return  string;}
  std::string const*  operator->() const{return &string;}

  std::string  to_string() const;

  std::string  release(){return std::move(string);}

  void  print() const;

};




}




#endif




