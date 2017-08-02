#include"pp_TokenString.hpp"
#include<cstdio>
#include<algorithm>




namespace preprocessing{




Token
TokenString::
null;


void
TokenString::
extend()
{
    if(!allocated_length)
    {
      pointer = new Token[8];

      allocated_length = 8;
    }

  else
    if((length+1) >= allocated_length)
    {
      auto  new_allocated_length = allocated_length*2;

      auto  new_pointer = new Token[new_allocated_length];

        for(int  i = 0;  i < allocated_length;  ++i)
        {
          new_pointer[i] = std::move(pointer[i]);
        }


               pointer = new_pointer         ;
      allocated_length = new_allocated_length;
    }
}


TokenString&
TokenString::
operator=(TokenString const&  rhs) noexcept
{
  clear();

  pointer = new Token[rhs.allocated_length+1];

  allocated_length = rhs.allocated_length;
            length = rhs.length          ;

    for(int  i = 0;  i < length;  ++i)
    {
      pointer[i] = rhs.pointer[i];
    }


  return *this;
}


TokenString&
TokenString::
operator=(TokenString&&  rhs) noexcept
{
  clear();

  std::swap(pointer,rhs.pointer);

  allocated_length = rhs.allocated_length;
            length = rhs.length          ;


  return *this;
}


void
TokenString::
operator+=(Token const&  tok)
{
    if(tok)
    {
      extend();

      back() = tok;

      length += 1;
    }
}


void
TokenString::
operator+=(Token&&  tok)
{
    if(tok)
    {
      extend();

      back() = std::move(tok);

      length += 1;
    }
}


void
TokenString::
operator+=(TokenString const&  rhs)
{
    for(auto&  tok: rhs)
    {
      (*this) += tok;
    }
}


void
TokenString::
operator+=(TokenString&&  rhs)
{
    for(auto&&  tok: rhs)
    {
      (*this) += std::move(tok);
    }
}




void
TokenString::
clear()
{
    if(pointer != &null)
    {
      delete[] pointer        ;
               pointer = &null;
    }


  allocated_length = 0;
            length = 0;
}


void
TokenString::
print() const
{
    for(auto&  tok: *this)
    {
      tok.print();

      printf("\n");
    }
}



}




