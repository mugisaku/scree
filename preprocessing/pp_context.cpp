#include"pp_context.hpp"
#include"pp_cursor.hpp"
#include"pp.hpp"
#include<cstdlib>
#include<ctime>




namespace preprocessing{




Context::
Context()
{
  Macro  date(std::string("__DATE__"));
  Macro  time(std::string("__TIME__"));


  auto  t = std::time(nullptr);

  auto  tm = std::gmtime(&t);

  char  buf[256];

  std::strftime(buf,sizeof(buf),"%a %d %Y",tm);

  date.set_token_string(TokenString(Token(TokenKind::string,std::string(buf))));

  std::strftime(buf,sizeof(buf),"%H:%M:%S",tm);

  time.set_token_string(TokenString(Token(TokenKind::string,std::string(buf))));


  append_macro(std::move(date));
  append_macro(std::move(time));


  append_macro(Macro(std::string("__cplusplus"),Token(0)));

  append_macro(Macro(std::string("__GNUC__"),1));
  append_macro(Macro(std::string("__STDC__"),1));
  append_macro(Macro(std::string("__STDC_VERSION__"),201112));
  append_macro(Macro(std::string("__cpp_exceptions"),199711));
}




void
Context::
remove_macro(std::string const&  name)
{
  auto   it = macro_table.begin();
  auto  end = macro_table.end();

    while(it != end)
    {
        if(*it == name)
        {
          macro_table.erase(it);

          break;
        }


      ++it;
    }
}


const Macro*
Context::
find_macro(const std::string&  name) const
{
    for(auto&  m: macro_table)
    {
        if(m == name)
        {
          return &m;
        }
    }


  return nullptr;
}


void
Context::
print() const
{
    for(auto&  m: macro_table)
    {
      m.print();

      printf("\n");
    }


  printf("インクルード・ディレクトリー・リスト\n");

    for(auto&  incdir: include_directory_list)
    {
      printf("%s%s\n",incdir.data(),(incdir.back() != '/')? "/":"");
    }


  printf("\n");
}


}




