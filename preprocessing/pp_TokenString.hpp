#ifndef PP_TokenString_HPP_INCLUDED
#define PP_TokenString_HPP_INCLUDED


#include"pp_token.hpp"


namespace preprocessing{


class
TokenString
{
  static Token  null;

  Token*  pointer=&null;

  size_t  allocated_length=0;

  size_t  length=0;

  void  extend();

public:
  using       iterator = Token      *;
  using const_iterator = Token const*;

  TokenString(){}
  TokenString(Token&&  tok){*this += std::move(tok);}
  TokenString(TokenString const&  rhs) noexcept{*this = rhs;}
  TokenString(TokenString&&       rhs) noexcept{*this = std::move(rhs);}
 ~TokenString(){clear();}


  TokenString&  operator=(TokenString const&  rhs) noexcept;
  TokenString&  operator=(TokenString&&       rhs) noexcept;

  void  operator+=(Token const&  tok);
  void  operator+=(Token&&       tok);
  void  operator+=(TokenString const&  rhs);
  void  operator+=(TokenString&&       rhs);

  void  clear();

  size_t  size() const{return length;}
  bool  empty() const{return !length;}

  void  pop_back(){length -= 1;}

  Token const&  front() const{return data()[       0];}
  Token const&   back() const{return data()[length-1];}

  Token&  front(){return data()[       0];}
  Token&   back(){return data()[length-1];}

  Token      *  data()      {return pointer;}
  Token const*  data() const{return pointer;}

  iterator  begin() const{return pointer;}
  iterator    end() const{return pointer+length;}

  const_iterator  cbegin() const{return pointer;}
  const_iterator    cend() const{return pointer+length;}

  void  print(FILE*  out=stdout, bool  newline=false) const;

};




}




#endif




