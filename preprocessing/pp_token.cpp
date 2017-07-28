#include"pp_token.hpp"
#include"pp_unicode.hpp"
#include"pp_TokenString.hpp"
#include<cstdio>
#include<algorithm>
#include<new>




namespace preprocessing{




Token::Token(unsigned int        i): kind(TokenKind::integer    ){data.integer     = i;}
Token::Token(         long       i): kind(TokenKind::l_integer  ){data.l_integer   = i;}
Token::Token(unsigned long       i): kind(TokenKind::ul_integer ){data.ul_integer  = i;}
Token::Token(         long long  i): kind(TokenKind::ll_integer ){data.ll_integer  = i;}
Token::Token(unsigned long long  i): kind(TokenKind::ull_integer){data.ull_integer = i;}
Token::Token(std::string&&     s): kind(TokenKind::string   ){new(&data) std::string(   std::move(s));}
Token::Token(std::u16string&&  s): kind(TokenKind::u16string){new(&data) std::u16string(std::move(s));}
Token::Token(std::u32string&&  s): kind(TokenKind::u32string){new(&data) std::u32string(std::move(s));}
Token::Token(char      c): kind(TokenKind::character   ){data.character    = c;}
Token::Token(char16_t  c): kind(TokenKind::u16character){data.u16character = c;}
Token::Token(char32_t  c): kind(TokenKind::u32character){data.u32character = c;}
Token::Token(Identifier&&  id): kind(TokenKind::identifier){new(&data) std::string(std::move(id.string));}
Token::Token(Keyword&&  kw): kind(TokenKind::keyword){new(&data) std::string(std::move(kw.string));}




Token&
Token::
operator=(const Token&  rhs) noexcept
{
  clear();

  kind = rhs.kind;

    switch(kind)
    {
  case(TokenKind::null):
      break;
  case(TokenKind::integer):
  case(TokenKind::l_integer):
  case(TokenKind::ul_integer):
  case(TokenKind::ll_integer):
  case(TokenKind::ull_integer):
  case(TokenKind::character):
  case(TokenKind::u16character):
  case(TokenKind::u32character):
      data.ull_integer = rhs.data.ull_integer;
      break;
  case(TokenKind::identifier):
  case(TokenKind::keyword):
  case(TokenKind::string):
      new(&data) std::string(rhs.data.string);
      break;
  case(TokenKind::u16string):
      new(&data) std::u16string(rhs.data.u16string);
      break;
  case(TokenKind::u32string):
      new(&data) std::u32string(rhs.data.u32string);
      break;
    }


  return *this;
}


Token&
Token::
operator=(Token&&  rhs) noexcept
{
  clear();

  std::swap(kind,rhs.kind);

    switch(kind)
    {
  case(TokenKind::null):
      break;
  case(TokenKind::integer):
  case(TokenKind::l_integer):
  case(TokenKind::ul_integer):
  case(TokenKind::ll_integer):
  case(TokenKind::ull_integer):
  case(TokenKind::character):
  case(TokenKind::u16character):
  case(TokenKind::u32character):
      data.ull_integer = rhs.data.ull_integer;
      break;
  case(TokenKind::identifier):
  case(TokenKind::keyword):
  case(TokenKind::string):
      new(&data) std::string(std::move(rhs.data.string));
      break;
  case(TokenKind::u16string):
      new(&data) std::u16string(std::move(rhs.data.u16string));
      break;
  case(TokenKind::u32string):
      new(&data) std::u32string(std::move(rhs.data.u32string));
      break;
    }


  return *this;
}




void
Token::
clear()
{
    switch(kind)
    {
  case(TokenKind::null):
  case(TokenKind::integer):
  case(TokenKind::l_integer):
  case(TokenKind::ul_integer):
  case(TokenKind::ll_integer):
  case(TokenKind::ull_integer):
  case(TokenKind::character):
  case(TokenKind::u16character):
  case(TokenKind::u32character):
      break;
  case(TokenKind::identifier):
  case(TokenKind::string):
  case(TokenKind::keyword):
      data.string.~basic_string();
      break;
  case(TokenKind::u16string):
      data.u16string.~basic_string();
      break;
  case(TokenKind::u32string):
      data.u32string.~basic_string();
      break;
    }


  kind = TokenKind::null;
}


void
Token::
print() const
{
    switch(kind)
    {
  case(TokenKind::null):
      printf(" NULL ");
      break;
  case(TokenKind::integer):
      printf(" %u(0x%08X)",data.integer,data.integer);
      break;
  case(TokenKind::l_integer):
      printf(" %ld(0x%016X)",data.l_integer,data.l_integer);
      break;
  case(TokenKind::ul_integer):
      printf(" %lu(0x%016X)",data.ul_integer,data.ul_integer);
      break;
  case(TokenKind::ll_integer):
      printf(" %lld(0x%016X)",data.ll_integer,data.ll_integer);
      break;
  case(TokenKind::ull_integer):
      printf(" %llu(0x%016X)",data.ull_integer,data.ull_integer);
      break;
  case(TokenKind::identifier):
  case(TokenKind::keyword):
      printf(" %s ",data.string.data());
      break;
  case(TokenKind::string):
      printf(" \"%s\" ",data.string.data());
      break;
  case(TokenKind::u16string):
      printf(" u\"");

        for(auto  c: data.u16string)
        {
          printf("%s",UTF8Chunk(c).codes);
        }


      printf("\" ");
      break;
  case(TokenKind::u32string):
      printf(" U\"");

        for(auto  c: data.u32string)
        {
          printf("%s",UTF8Chunk(c).codes);
        }


      printf("\" ");
      break;
  case(TokenKind::character):
      printf(" \'%c\' ",data.character);
      break;
  case(TokenKind::u16character):
      printf(" u\'%s\' ",UTF8Chunk(data.u16character).codes);
      break;
  case(TokenKind::u32character):
      printf(" U\'%s\' ",UTF8Chunk(data.u16character).codes);
      break;
    }
}




}




