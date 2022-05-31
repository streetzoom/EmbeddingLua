#include <iostream>
#include <string>

// include Lua, assumes it is local to this file
extern "C"
{
#include "Lua542/include/lua.h"
#include "Lua542/include/lauxlib.h"
#include "Lua542/include/lualib.h"
}

// Link to lua library
#ifdef _WIN64
#pragma comment(lib, "Lua542/liblua54.a")
#endif


// Little error checking utility function
bool CheckLua(lua_State *L, int r)
{
    if (r != LUA_OK)
    {
        std::string errormsg = lua_tostring(L, -1);
        std::cout << errormsg << std::endl;
        return false;
    }
    return true;
}


int lua_HostFunction(lua_State *L)
{
    float a = (float)lua_tonumber(L, 1);
    float b = (float)lua_tonumber(L, 2);
    std::cout << "[C++] HostFunction(" << a << ", " << b << ") called" << std::endl;
    float c = a * b;
    lua_pushnumber(L, c);
    return 1;
}


int main()
{
    struct Player
    {
        std::string title;
        std::string name;
        std::string family;
        int level;
    }player;

    // Create Lua State
    lua_State *L = luaL_newstate();

    // Add standard libraries to Lua Virtual Machine
    luaL_openlibs(L);

    // Register our C++ Function in the global Lua space
    lua_register(L,"HostFunction", lua_HostFunction);

    // Load and parse the Lua File
    if (CheckLua(L, luaL_dofile(L, "Test.lua")))
    {
        lua_getglobal(L, "DoAThing");
        if (lua_isfunction(L, -1))
        {
            lua_pushnumber(L, 5.0f);
            lua_pushnumber(L, 6.0f);
            if (CheckLua(L, lua_pcall(L, 2, 1, 0)))
            {
                std::cout << "[C++] Called in Lua 'DoAThing(5.0f, 6.0f)', got " << (float)lua_tonumber(L, -1) << std::endl;
            }
        }

        // if (lua_istable(L, -1))
        // {
        //     lua_pushstring(L, "Name");
        //     lua_gettable(L, -2);
        //     player.name = lua_tostring(L, -1);
        //     lua_pop(L, 1);
        //
        //     lua_pushstring(L, "Family");
        //     lua_gettable(L, -2);
        //     player.family = lua_tostring(L, -1);
        //     lua_pop(L, 1);
        //
        //     lua_pushstring(L, "Title");
        //     lua_gettable(L, -2);
        //     player.title = lua_tostring(L, -1);
        //     lua_pop(L, 1);
        //
        //     lua_pushstring(L, "Level");
        //     lua_gettable(L, -2);
        //     player.level = lua_tonumber(L, -1);
        //     lua_pop(L, 1);
        // }
        // std::cout << player.title << " " << player.name << " of " << player.family << " [Lvl: " << player.level << "]" << std::endl;
    }

    system("pause"); // i know its bad, dont worry
    lua_close(L);
    return 0;
}
