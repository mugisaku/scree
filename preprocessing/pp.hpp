#ifndef PP_HPP_INCLUDED
#define PP_HPP_INCLUDED


#include"pp_cursor.hpp"
#include"pp_token.hpp"
#include"pp_TokenString.hpp"
#include"pp_context.hpp"
#include<string>
#include<cstdio>
#include<algorithm>


namespace preprocessing{


void  skip_space(             Cursor&  cur);
void  skip_spaces(            Cursor&  cur);
void  skip_spaces_and_newline(Cursor&  cur);

void  skip_blockstyle_comment(Cursor&  cur);
void  skip_linestyle_comment( Cursor&  cur);



std::string  read_quoted(    Cursor&  cur, char  key_char);
std::string  read_identifier(Cursor&  cur                );


std::string   read_argument(     Cursor&  cur);
ArgumentList  read_argument_list(Cursor&  cur);



std::string     read_directive(Cursor&  cur);
TokenString  process_directive(Cursor  cur, Context&  ctx);

TokenString  read_include(Cursor&  cur, Context&  ctx);
void          read_define(Cursor&  cur, Context&  ctx);


TokenString  process_identifier(Token&&  id, Cursor&  cur, Context const&  ctx);

Token  read_token(Cursor&  cur);

TokenString  process_file(std::string const&  s, Context&        ctx);
TokenString  process_text(std::string const&  s, Context const&  ctx, ArgumentList const*  argls=nullptr);


#define report  printf("[%s %s %4d]\n",__FILE__,__func__,__LINE__)

inline void  print(std::string const&  s){printf("[print]%s\n",s.data());}


}




#endif




