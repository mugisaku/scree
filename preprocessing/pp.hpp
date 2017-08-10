#ifndef PP_HPP_INCLUDED
#define PP_HPP_INCLUDED


#include"pp_cursor.hpp"
#include"pp_token.hpp"
#include"pp_TokenString.hpp"
#include"pp_context.hpp"
#include"pp_expression.hpp"
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


Expression  make_expression(TokenString::const_iterator&  it, Context const&  ctx);
Expression  make_expression(char const*  text, Context const&  ctx);


bool  is_u(char  c);
bool  is_l(char  c);
bool  is_ll(char const*  p);
bool  is_ul(char const*  p);
bool  is_integer_suffix(std::string const&  s);


Token  read_token(Cursor&  cur);


TokenString  tokenize_main_text(char const*  s, char const*  file_path);
TokenString  tokenize_sub_text( char const*  s                        );


bool  process_identifier(TokenString::iterator&  it, TokenString&  buf, Context const&  ctx,
                         Macro const*  parent=nullptr, ArgumentList const*  args=nullptr);

void  process_token_string_that_includes_directives(TokenString&  toks, Context&        ctx);
void  process_token_string_for_expression(          TokenString&  toks, Context const&  ctx);
void  process_token_string(                         TokenString&  toks, Context const&  ctx);


#define report  printf("[%s %s %4d]\n",__FILE__,__func__,__LINE__)

inline void  print(std::string const&  s){printf("[print]%s\n",s.data());}


}




#endif




