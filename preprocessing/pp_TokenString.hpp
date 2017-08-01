#ifndef PP_TokenString_HPP_INCLUDED
#define PP_TokenString_HPP_INCLUDED


#include<vector>
#include"pp_token.hpp"


namespace preprocessing{


class
TokenString: public std::vector<Token>
{
public:
  void  operator+=(TokenString&&  rhs)
  {
      if(size() && !back())
      {
        pop_back();
      }


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
      for(auto&  tok: *this)
      {
        tok.print();

        printf("\n");
      }
  }

};




}




#endif




