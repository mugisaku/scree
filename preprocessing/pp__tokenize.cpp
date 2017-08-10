#include<string>
#include<cstdlib>
#include<cstring>
#include<cctype>
#include<vector>
#include<getopt.h>
#include"pp.hpp"


namespace preprocessing{


namespace{
std::string
read_directive(Cursor&  cur)
{
  std::string  s;

    for(;;)
    {
      auto  const c = *cur;

        if(c == '\n')
        {
          break;
        }

      else
        if((c ==  ' ') ||
           (c == '\t') ||
           (c == '\r'))
        {
          cur += 1;

          s.push_back(' ');
        }

      else
        if(cur.compare("\\\n"))
        {
          cur += 1;

          cur.newline();
        }

      else
        if(iscntrl(c))
        {
          throw Error(cur,"ディレクティブの途中で制御文字");
        }

      else
        {
          cur += 1;

          s.push_back(c);
        }
    }


  return std::move(s);
}
}


TokenString
tokenize_sub_text(char const*  s)
{
  Cursor  cur(s);

  TokenString  toks;

  TokenKind  last_k = TokenKind::null;

    for(;;)
    {
      skip_spaces(cur);

      auto  tok = read_token(cur);

        if(!tok)
        {
          break;
        }

      else
        {
          auto  k = last_k                 ;
                    last_k = tok.get_kind();

            if((tok == TokenKind::identifier) &&
               Token::is_integer(k) &&
               is_integer_suffix(*tok))
            {
              toks.back().set_suffix(*tok);
            }

          else
            {
              toks += std::move(tok);
            }
        }
    }


  return std::move(toks);
}


TokenString
tokenize_main_text(char const*  s, char const*  file_path)
{
  Cursor  cur(s,file_path);

  TokenString  toks;

    if(*cur == '#')
    {
      TokenInfo  info(cur);

      cur += 1;

      toks += Token(TokenKind::directive,read_directive(cur),std::move(info));
    }


    while(*cur)
    {
        if(cur.compare('\n','#'))
        {
          cur.newline();

          TokenInfo  info(cur);

          cur += 1;

          toks += Token(TokenKind::directive,read_directive(cur),std::move(info));
        }

      else
        if(cur.compare('\n'))
        {
          cur.newline();
        }

      else
        {
          skip_spaces(cur);

          toks += read_token(cur);
        }
    }


  return std::move(toks);
}


}




