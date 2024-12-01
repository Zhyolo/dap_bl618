#include "shell.h"
#include <stdint.h>
#include <stdio.h>
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"

#define printf(fmt, ...) shellPrint(shellGetCurrent(), fmt, ##__VA_ARGS__)

static int lua(uint8_t argc, char **argv)
{
    if(argc != 2)
    {
        printf("usage: lua <script>\n");
        return 0;
    }

    lua_State *L = luaL_newstate();
    luaL_openlibs(L);

    int ret = luaL_dostring(L, argv[1]);
    
    lua_close(L);
    return ret;
}
SHELL_EXPORT_CMD(SHELL_CMD_PERMISSION(0) | SHELL_CMD_TYPE(SHELL_TYPE_CMD_MAIN), lua, lua, lua);
