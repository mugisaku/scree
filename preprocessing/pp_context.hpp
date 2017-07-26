#ifndef PP_Context_HPP_INCLUDED
#define PP_Context_HPP_INCLUDED


#include"pp_macro.hpp"


namespace preprocessing{


enum class
ContextState
{
  running,
  looking_for_next_block,
  looking_for_endif,

};


struct
Context
{
  std::list<Macro>  macro_table;

  std::vector<ContextState>  state_stack;

public:
  Macro const*  find_macro(const std::string&  name) const;

  void    push_state(ContextState  st){state_stack.emplace_back(st);}
  bool  change_state(ContextState  st){if(state_stack.size()){state_stack.back() = st;} return false;}
  bool     pop_state(){if(state_stack.size()){state_stack.pop_back();  return true;} return false;}

  ContextState  get_state() const{return state_stack.size()? state_stack.back()
                                                           : ContextState::running;}

  void  append_macro(Macro&&  macro){macro_table.emplace_back(std::move(macro));}
  void  remove_macro(std::string const&  name);

};


}




#endif




