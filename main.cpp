#include "game.h"

int main(int argc, char* argv[])
{	

	lua_State *L = luaL_newstate();
	luaL_openlibs(L);
	luaL_dofile(L, "script.lua");

	//Create game and start.
	Game* game = new Game();
	game->Run();
	delete game;  //Deallocate game from memory
}