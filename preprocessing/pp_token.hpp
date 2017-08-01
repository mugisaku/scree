#ifndef PP_Token_HPP_INCLUDED
#define PP_Token_HPP_INCLUDED


#include<string>


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

  operator bool() const{return kind != TokenKind::null;}

  std::string const&  operator* () const{return  string;}
  std::string const*  operator->() const{return &string;}

  void  print() const;

};




}




#endif




