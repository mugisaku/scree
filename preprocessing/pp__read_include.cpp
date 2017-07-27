#include"pp.hpp"


namespace preprocessing{


namespace{


std::string
read_path(Cursor&  cur)
{
  std::string  s;

    for(;;)
    {
      auto  const c = *cur;

        if(isalnum(c) || (c == '_') || (c == '/') || (c == '.'))
        {
          cur += 1;

          s.push_back(c);
        }

      else
        {
          break;
        }
    }


  return std::move(s);
}


}



TokenList
read_include(Cursor  cur, Context&  ctx)
{
  std::string  content;
  std::string     path;

  skip_spaces(cur);

    if(*cur == '<')
    {
      cur += 1;

      skip_spaces(cur);
    }

  else
    if(*cur == '\"')
    {
      cur += 1;

      skip_spaces(cur);
    }


  path = read_path(cur);


  auto  f = fopen(path.data(),"rb");

    if(!f)
    {
      throw Error(cur,"インクルード指定されたパスが開けない");
    }


    for(;;)
    {
      auto  const c = fgetc(f);

        if(feof(f))
        {
          break;
        }

      else
        if(ferror(f))
        {
          fclose(f);

          throw Error(cur,"インクルードファイルを読み込み中にエラー");
        }


      content.push_back(c);
    }


  fclose(f);

  Cursor  cocur(content);

  return process_main(cocur,ctx);
}




}




