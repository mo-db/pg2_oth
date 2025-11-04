#include <iostream>
#include "luajit_fin/include/luajit-2.1/lua.hpp"


// validation is important -> the script could return anything
bool check_lua(lua_State *L, int result) {
	if (result != LUA_OK) {
		std::string error_msg = lua_tostring(L, -1);
		std::cout << "[ERROR] -> " << error_msg << std::endl;
		return false;
	}
	return true;
}

int lua_HostFunction(lua_State *L) {
	// use lua_gettop() to query how many elements are on the stack
	
	float a = (float)lua_tonumber(L, 1);
	float b = (float)lua_tonumber(L, 2);
	std::cout << "HostFunction has been called!" << std::endl;

	float result = a + b;
	
	lua_pushnumber(L, result);

	// return the amount of return-arguments
	return 1;
}


int main() {
	lua_State *L = luaL_newstate(); // creates a lua VM?
	luaL_openlibs(L); // adds basic libs to VM
	
	lua_register(L, "HostFunction", lua_HostFunction);

	try {
		// ---- MAIN ----
		std::string string_to_parse {"a = 7 + 11 + math.sin(2)"};
		// TODO: what exactly is this state?

		// C-API function
		if (check_lua(L, luaL_dofile(L, "test_script.lua"))){
			// first example, lua is not typed
			lua_getglobal(L, "a");
			if (lua_isnumber(L, -1)) {
				float c_a = (float)lua_tonumber(L, -1);
				std::cout << "the parsed value: " << c_a << std::endl;
			}


			// second example
			lua_getglobal(L, "Bar");
			if (lua_isfunction(L, -1)) {
				lua_pushnumber(L, 3);
				lua_pushnumber(L, 2);
				if (check_lua(L, lua_pcall(L, 2, 1, 0))) {
					// lua has poped the two variables of the stack
					// result is now on top of the stack
					std::cout << "got from lua: " << lua_tonumber(L, -1) << std::endl;
				}
				// if there was an error, the error value is on the stack
			}
		}



		// ---- END MAIN ----
	} catch(const std::exception &e) {
		std::cout << "ERROR: " << e.what() << std::endl;
		return 1;
	}
	lua_close(L);
	return 0;
}
