#ifndef PP_Token_HPP_INCLUDED
#define PP_Token_HPP_INCLUDED


#include<string>
#include<cstdint>
#include<vector>


namespace preprocessing{


struct
Identifier
{
  std::string  string;

  Identifier(std::string&&  s): string(std::move(s)){}
};


struct
Keyword
{
  std::string  string;

  Keyword(std::string&&  s): string(std::move(s)){}

};


struct
Operator
{
  char  codes[4];

  constexpr Operator(char  c0=0, char  c1=0, char  c2=0): codes{c0,c1,c2,0}{}

  constexpr uint32_t  operator*() const{return((codes[0]<<24)|(codes[1]<<16)|(codes[2]<<8));}

};


struct TokenString;


enum class
TokenKind
{
  null,
  integer,
  l_integer,
  ll_integer,
  ul_integer,
  ull_integer,
  character,
  u16character,
  u32character,
  string,
  u16string,
  u32string,
  identifier,
  keyword,
  operator_,

  token_string,

};


union
TokenData
{
  unsigned int            integer;
           long         l_integer;
  unsigned long        ul_integer;
           long long   ll_integer;
  unsigned long long  ull_integer;

  char         character;
  char16_t  u16character;
  char32_t  u32character;

  std::string          string;
  std::u16string    u16string;
  std::u32string    u32string;

  Operator  operator_;

  TokenString*  token_string;

   TokenData(){}
  ~TokenData(){}

};



class
Token
{
  TokenKind  kind=TokenKind::null;
  TokenData  data;

public:
  Token(){}
  explicit Token(unsigned int          i);
  explicit Token(         long         l);
  explicit Token(unsigned long        ul);
  explicit Token(         long long   ll);
  explicit Token(unsigned long long  ull);
  explicit Token(std::string&&     s);
  explicit Token(std::u16string&&  s);
  explicit Token(std::u32string&&  s);
  explicit Token(char      c);
  explicit Token(char16_t  c);
  explicit Token(char32_t  c);
  explicit Token(Identifier&&  id);
  explicit Token(Keyword&&  kw);
  Token(const Token&   rhs) noexcept{*this = rhs;}
  Token(      Token&&  rhs) noexcept{*this = std::move(rhs);}
 ~Token(){clear();}

  Token&  operator=(const Token&   rhs) noexcept;
  Token&  operator=(      Token&&  rhs) noexcept;

  bool  operator==(TokenKind  k) const{return kind == k;}

  operator bool() const{return kind != TokenKind::null;}

  const TokenData*  operator->() const{return &data;}

  void  clear();

  void  print() const;

  static std::string  move_string(Token&&  tok){return std::move(tok.data.string);}
  static void  change_identifier_to_keyword(Token&  tok){tok.kind = TokenKind::keyword;}

};




}




#endif




