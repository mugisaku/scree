#ifndef PP_HPP_INCLUDED
#define PP_HPP_INCLUDED


#include"pp_cursor.hpp"
#include"pp_token.hpp"
#include"pp_TokenString.hpp"
#include"pp_context.hpp"
#include<string>
#include<cstdio>
#include<algorithm>
#include<cctype>


namespace preprocessing{


void  skip_space(             Cursor&  cur);
void  skip_spaces(            Cursor&  cur);
void  skip_spaces_and_newline(Cursor&  cur);

void  skip_blockstyle_comment(Cursor&  cur);
void  skip_linestyle_comment( Cursor&  cur);



std::string  read_quoted(    Cursor&  cur, char  key_char);
std::string  read_identifier(Cursor&  cur                );

constexpr bool  isident0(char  c){return(isalpha(c) || (c == '_'));}
constexpr bool  isidentn(char  c){return(isalnum(c) || (c == '_'));}


ArgumentList  read_argument_list(TokenString::const_iterator&  cur, Context const&  ctx);



TokenString  read_include(Cursor&  cur, Context&  ctx);
void          read_define(Cursor&  cur, Context&  ctx);


Token  read_token(Cursor&  cur);

TokenString  process_identifier(std::string const&  id, TokenString::const_iterator&  it, Context const&  ctx, Macro const*  parent=nullptr);

TokenString  process_file(std::string const&  s);
TokenString  process_text(std::string const&  s);

TokenString  process_token_string_that_includes_directives(TokenString const&  src, Context&        ctx);
TokenString  process_token_string(                         TokenString const&  src, Context const&  ctx);


#define report  printf("[%s %s %4d]\n",__FILE__,__func__,__LINE__)

inline void  print(std::string const&  s){printf("[print]%s\n",s.data());}


}




#endif




