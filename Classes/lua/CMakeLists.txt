set(LUA_SRC
  Classes/lua/lapi.c
  Classes/lua/lauxlib.c
  Classes/lua/lbaselib.c
  Classes/lua/lcode.c
  Classes/lua/ldblib.c
  Classes/lua/ldebug.c
  Classes/lua/ldo.c
  Classes/lua/ldump.c
  Classes/lua/lfunc.c
  Classes/lua/lgc.c
  Classes/lua/linit.c
  Classes/lua/liolib.c
  Classes/lua/llex.c
  Classes/lua/lmathlib.c
  Classes/lua/lmem.c
  Classes/lua/loadlib.c
  Classes/lua/lobject.c
  Classes/lua/lopcodes.c
  Classes/lua/loslib.c
  Classes/lua/lparser.c
  Classes/lua/lstate.c
  Classes/lua/lstring.c
  Classes/lua/lstrlib.c
  Classes/lua/ltable.c
  Classes/lua/ltablib.c
  Classes/lua/ltm.c
  Classes/lua/lundump.c
  Classes/lua/lvm.c
  Classes/lua/lzio.c
  Classes/lua/print.c
)

add_library(lua STATIC
  ${LUA_SRC}
)

set_target_properties(lua
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
)
