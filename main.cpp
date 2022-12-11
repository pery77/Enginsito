#include "game.h"

int main(int argc, char* argv[])
{	
	//Create game and start.
	Game* game = new Game();
	game->Run();
	delete game;  //Deallocate game from memory
}