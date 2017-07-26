#ifndef PP_HPP_INCLUDED
#define PP_HPP_INCLUDED


#include"cursor.hpp"
#include"pp_context.hpp"
#include"pp_expression.hpp"
#include<string>
#include<cstdio>
#include<algorithm>


namespace preprocessing{


void  read_spaces(Cursor&  cur);
void  read_spaces_and_newline(Cursor&  cur);
void  read_spaces_and_newline_with_escape(Cursor&  cur);


std::string  read_identifier(Cursor&  cur);


std::string  read_directive(Cursor&  cur);
std::string  process_directive(Cursor  cur, Context&  ctx);


std::string  process_main(Cursor  cur);


}




#endif




