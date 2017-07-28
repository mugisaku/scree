#ifndef PP_TokenString_HPP_INCLUDED
#define PP_TokenString_HPP_INCLUDED


#include<vector>
#include"pp_token.hpp"


namespace preprocessing{


class
TokenString: public std::vector<Token>
{
  char  head_character;
  char  tail_character;

public:
  TokenString(char  head=0, char  tail=0): head_character(head), tail_character(tail){}

  void  operator+=(TokenString&&  rhs)
  {
      for(auto&&  tok: rhs)
      {
          if(tok)
          {
            this->emplace_back(std::move(tok));
          }
      }
  }

  void  print() const
  {
    if(head_character){printf("%c",head_character);}

      for(auto&  tok: *this)
      {
        tok.print();

        printf("\n");
      }


    if(tail_character){printf("%c",tail_character);}
  }

};




}




#endif




