#include"pp.hpp"
#include"pp_macro.hpp"
#include"cursor.hpp"
#include<cctype>




namespace preprocessing{


namespace{
std::string
concatenate(Cursor&  cur)
{
  std::string  s;

  read_spaces_and_newline_with_escape(cur);

    while(s.size() && (s.back() == ' '))
    {
      s.pop_back();
    }


    for(;;)
    {
      auto  const  c = *cur;

        if(iscntrl(c))
        {
          throw Error(cur,"字句連結中に制御文字");
        }

      else
        if((isalnum(c) || ispunct(c)) || (c == '#'))
        {
          s.push_back(c);

          cur += 1;
        }

      else
        {
          break;
        }
    }


  return std::move(s);
}


std::string
stringize(Cursor&  cur)
{
  std::string  s;

  read_spaces_and_newline_with_escape(cur);

  s.push_back('\"');

    for(;;)
    {
      auto  const  c = *cur;

        if(iscntrl(c))
        {
          throw Error(cur,"字句連結中に制御文字");
        }

      else
        if((isalnum(c) || ispunct(c)) || (c == '#'))
        {
          s.push_back(c);

          cur += 1;
        }

      else
        {
          s.push_back('\"');

          break;
        }
    }


  return std::move(s);
}


}


std::string
Macro::
replace_text(ArgumentList&  argls) const
{
    if(!function_style_flag)
    {
      throw Error(Cursor(),"関数式マクロではない");
    }


    if(!variable_arguments_flag && (parameter_list.size() != argls.size()))
    {
      throw Error(Cursor(),"引数の数が一致しません");
    }


  std::string  new_text;
  std::string         s;

  Cursor  cur(text);

  enum class Operation{
    none,
    concatenate,
    stringize,

  } op=Operation::none;

    for(;;)
    {
      auto  const c = *cur;

        if(!c)
        {
          break;
        }

      else
        if(isalpha(c) || (c == '_'))
        {
          auto  id = read_identifier(cur);

          auto  i = find_parameter(id->string);

            if(i < 0)
            {
              new_text.append(id->string);
            }

          else
            {
              new_text.append(argls[i]);
            }
        }

      else
        if(cur.compare("##"))
        {
          cur += 2;

//          s.append(concatenate(cur));
        }

      else
        if(c == '#')
        {
          cur += 1;

          s.append(stringize(cur));
        }

      else
        {
          cur += 1;

          new_text.push_back(c);
        }
    }


  return std::move(new_text);
}



}




