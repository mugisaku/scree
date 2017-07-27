#ifndef PP_HPP_INCLUDED
#define PP_HPP_INCLUDED


#include"cursor.hpp"
#include"pp_token.hpp"
#include"pp_context.hpp"
#include"pp_expression.hpp"
#include<string>
#include<cstdio>
#include<algorithm>


namespace preprocessing{


void  read_spaces(Cursor&  cur);
void  read_spaces_and_newline(Cursor&  cur);
void  read_spaces_and_newline_with_escape(Cursor&  cur);


Token  read_number_literal_that_begins_by_zero(Cursor&  cur);
Token  read_decimal_number_literal(            Cursor&  cur);
Token  read_string_literal(   Cursor&  cur);
Token  read_character_literal(Cursor&  cur);
Token  read_identifier(Cursor&  cur);
//Token  read_literal(   Cursor&  cur);


std::string   read_argument(     Cursor&  cur);
ArgumentList  read_argument_list(Cursor&  cur);


void  read_blockstyle_comment(Cursor&  cur);
void  read_linestyle_comment( Cursor&  cur);

void  skip_spaces(Cursor&  cur);

std::string   read_directive(Cursor&  cur);
TokenList  process_directive(Cursor  cur, Context&  ctx);

TokenList  read_include(Cursor  cur, Context&  ctx);
void  read_define(Cursor  cur, Context&  ctx);


TokenList  process_main(Cursor  cur, Context&  ctx);


#define report  printf("[%s %s %4d]\n",__FILE__,__func__,__LINE__)

inline void  print(std::string const&  s){printf("[print]%s\n",s.data());}


}




#endif




