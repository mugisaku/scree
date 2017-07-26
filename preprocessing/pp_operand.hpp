#ifndef PP_Operand_HPP_INCLUDED
#define PP_Operand_HPP_INCLUDED


#include<string>
#include<cstdint>


namespace preprocessing{


struct Expression;
struct Context;


struct
Defined
{
  std::string  identifier;

  Defined(std::string&&  id): identifier(std::move(id)){}

};


enum class
OperandKind
{
  null,
  integer,
  identifier,
  defined,
  expression,

};


union
OperandData
{
  unsigned int      integer;
  std::string    identifier;

  Expression*  expression;

   OperandData(){}
  ~OperandData(){}

};



class
Operand
{
  OperandKind  kind=OperandKind::null;
  OperandData  data;

public:
  Operand(){}
  explicit Operand(unsigned int  i);
  explicit Operand(std::string&&  id);
  explicit Operand(Defined&&  de);
  explicit Operand(Expression*  expr);
  Operand(const Operand&   rhs) noexcept{*this = rhs;}
  Operand(      Operand&&  rhs) noexcept{*this = std::move(rhs);}
 ~Operand(){clear();}

  Operand&  operator=(const Operand&   rhs) noexcept;
  Operand&  operator=(      Operand&&  rhs) noexcept;

  bool  operator==(OperandKind  k) const{return kind == k;}

  operator bool() const{return kind != OperandKind::null;}

  const OperandData*  operator->() const{return &data;}

  void  clear();

  int  get_value(Context const&  ctx) const;

  void  print() const;

};




}




#endif




