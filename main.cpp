#include "engine.h"

int main(int argc, char* argv[])
{	
	//Create engine and start.
	Engine* engine = new Engine();
	engine->Go();

	delete engine;  //Deallocate engine from memory
}