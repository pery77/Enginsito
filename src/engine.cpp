#include "engine.h"

#include "mb_manager.h"
#include "postProcessing.h"
#include "bios.h"

FilePathList droppedFiles = { 0 };

MBManager* basicIntepreter;
PostProcessing* postProcessing;
Bios* bios;

Engine::Engine()
{
    postProcessing = new PostProcessing();
    basicIntepreter = new MBManager(postProcessing);
    bios = new Bios(this);
}

Engine::~Engine()
{
}

void Engine::DropFileUpdate()
{
    if (!IsFileDropped()) return;
    
    if (droppedFiles.count > 0) UnloadDroppedFiles(droppedFiles);

    droppedFiles = LoadDroppedFiles();

    if (droppedFiles.count > 0)
    {
        char* firstFilePath = droppedFiles.paths[0];
        Tools::console->AddLog("[WARN] Dropped file: %s\n", firstFilePath);
    }
}

