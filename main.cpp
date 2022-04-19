#include "engine.h"

int main(int argc, char* argv[])
{	
	//Create engine and start.
	pery::Engine* engine = new pery::Engine();
	engine->Go();

	delete engine;  //Deallocate engine from memory
}