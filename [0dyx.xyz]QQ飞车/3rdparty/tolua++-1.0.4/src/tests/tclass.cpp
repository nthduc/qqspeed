extern "C" {
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

#include "tclass.h"

Test::Tst_A* Test::Tst_A::last;
Test::Tst_B* Test::Tst_B::last;
Test::Tst_C* Test::Tst_C::last;

int main ()
{
	Test::Tst_B* b = new Test::Tst_B;         // instance used in Lua code
	int  tolua_tclass_open (lua_State*);

	lua_State* L = lua_open();
	luaopen_base(L);
	tolua_tclass_open(L);

	lua_dofile(L,"tclass.lua");

	lua_close(L);

	delete b;
	return 0;
}

