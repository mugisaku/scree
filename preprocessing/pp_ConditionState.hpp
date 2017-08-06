#ifndef PP_ConditionState_HPP_INCLUDED
#define PP_ConditionState_HPP_INCLUDED


#include"pp_cursor.hpp"
#include"pp_token.hpp"
#include"pp_TokenString.hpp"
#include"pp_context.hpp"
#include<string>
#include<cstdio>
#include<algorithm>
#include<cctype>


namespace preprocessing{


enum class
IfDirectiveKind
{
  if_,
  ifdef,
  ifndef,
  elif,
  else_,
  endif,

};


class
ConditionState
{
  std::vector<IfDirectiveKind>  if_stack;

  struct StackFrame{
    bool  closed;
    bool  locked;

    constexpr StackFrame(bool  locked_=false): closed(false), locked(locked_){}

  };


  std::vector<StackFrame>  frames;

public:
  ConditionState();

  operator bool() const{return !frames.back().locked;}

  bool  is_locked() const{return frames.back().locked;}
  bool  is_not_closed() const{return !frames.back().closed;}

  void  enter();
  void   skip();

  int  get_depth() const{frames.size();}

  void  check(IfDirectiveKind  k, Context const&  ctx, char const*  expression);

};


}




#endif




