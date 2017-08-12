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

        if(isalnum(c) ||
           (c == '_') ||
           (c == '+') ||
           (c == '-') ||
           (c == '/') ||
           (c == '.'))
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


std::pair<FILE*,std::string>
open_file(std::string const&  src_path, std::vector<std::string> const&  incdir_list)
{
  std::vector<std::string>  failed_list;

    for(auto&  incdir: incdir_list)
    {
      std::string  path = incdir;

        if(path.back() != '/')
        {
          path += '/';
        }


      path += src_path;

      auto  f = fopen(path.data(),"rb");

        if(f)
        {
          return std::make_pair(f,std::move(path));
        }


      failed_list.emplace_back(std::move(path));
    }


    for(auto&  path: failed_list)
    {
      printf("%s\n",path.data());
    }


  printf("上記のいずれでも、ファイルは開けなかった\n");

  return std::make_pair<FILE*,std::string>(nullptr,std::string());
}
}



TokenString
read_include(Cursor&  cur, Context&  ctx)
{
  std::string  content;
  std::string     path;

  bool  system_flag = false;

    if(*cur == '<')
    {
      cur += 1;

      system_flag = true;
    }

  else
    if(*cur == '\"')
    {
      cur += 1;
    }

  else
    {
      throw Error(cur,"ファイル名が囲まれていない");
    }


  skip_spaces(cur);

  path = read_path(cur);


  auto  res = open_file(path,ctx.get_include_directory_list());

  auto  f = res.first;

    if(!f)
    {
      throw Error(cur,"%sが開けない",path.data());
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


  auto&  held_path = ctx.hold_string(std::move(res.second));

  auto  toks = tokenize_main_text(content.data(),&held_path);

  printf("try include %s\n",held_path.data());

    try
    {
      process_token_string_that_includes_directives(toks,ctx);
    }


    catch(Error&  e)
    {
      printf("インクルードエラー %s\n",held_path.data());

      throw;
    }


  return std::move(toks);
}




}




