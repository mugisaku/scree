cmake_minimum_required(VERSION 2.8)


if(DEFINED SLP)
else()
  set(SLP "0")
endif()


find_program(CCACHE_FOUND ccache)
if(CCACHE_FOUND)
  set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE "  sleep ${SLP};  CCACHE_DIR=/tmp/ccachedir ccache")
endif(CCACHE_FOUND)


set(CMAKE_VERBOSE_MAKEFILE ON)


if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug)
endif()


set(CMAKE_C_COMPILER  gcc)
set(CMAKE_C_FLAGS   "-std=c14 -Werror")
set(CMAKE_CXX_COMPILER  "g++")
set(CMAKE_CXX_FLAGS "-std=c++14 -Werror")
set(CMAKE_CXX_LINK_FLAGS  "-lstdc++ -lz -lpng -lSDL2 -lSDL2_image -lSDL2main")


set(CMAKE_C_FLAGS_RELEASE   "${CMAKE_C_FLAGS}   -O2 -march=core2 -fomit-frame-pointer")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} -O2 -march=core2 -fomit-frame-pointer")
set(CMAKE_C_FLAGS_DEBUG     "${CMAKE_C_FLAGS}   -O0 -g")
set(CMAKE_CXX_FLAGS_DEBUG   "${CMAKE_CXX_FLAGS} -O0 -g")


if(EM)
  set(EXTENSION ".html")
  set(CMAKE_CXX_COMPILER  em++)
  set(CMAKE_CXX_FLAGS "-std=c++14 -Werror -Wno-switch -O2")
  set(CMAKE_CXX_FLAGS_DEBUG "")
  set(CMAKE_CXX_LINK_FLAGS  "${CMAKE_CXX_LINK_FLAGS} -s USE_SDL=2 -s ALLOW_MEMORY_GROWTH=1")
endif()




set(SOURCES
  operand.cpp
  expression.cpp
  preprocessing/pp_unicode.cpp
  preprocessing/pp_token.cpp
  preprocessing/pp_TokenString.cpp
  preprocessing/pp_macro.cpp
  preprocessing/pp_macro__expand.cpp
  preprocessing/pp_context.cpp
  preprocessing/pp_cursor.cpp
  preprocessing/pp_expression.cpp
  preprocessing/pp_ConditionState.cpp
  preprocessing/pp.cpp
  preprocessing/pp__tokenize.cpp
  preprocessing/pp__skip.cpp
  preprocessing/pp__is.cpp
  preprocessing/pp__process_token_string.cpp
  preprocessing/pp__process_identifier.cpp
  preprocessing/pp__read_include.cpp
  preprocessing/pp__read_define.cpp
  preprocessing/pp__read_token.cpp
  preprocessing/pp__read_argument.cpp
  preprocessing/pp__make_expression.cpp
)


include_directories(.)
include_directories(./preprocessing)
add_executable("pp${EXTENSION}" ${SOURCES} ${EXPREE_SOURCES})




