#include "engine.h"

#include "mb_manager.h"
#include "postProcessing.h"
#include "bios.h"
#include "FileWatcher.h"

FilePathList droppedFiles = { 0 };

MBManager* basicIntepreter;
PostProcessing* postProcessing;
Bios* bios;
FileWatcher* fw;

Engine::Engine()
{
    std::stringstream ss;
    ss << "./" << ASSETS_FOLDER << "/";
    fw = new FileWatcher{ss.str(), 3.0f};
}

Engine::~Engine()
{
    basicIntepreter->CloseBas();
    delete postProcessing;
    delete basicIntepreter;
    delete bios;
}

void Engine::Init()
{
    postProcessing = new PostProcessing();
    basicIntepreter = new MBManager(this);
    bios = new Bios(this);

    //DISABLED WREN AT MOMENT
    //WrenManager* wren = new WrenManager();
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

void Engine::UpdateFileWatcher()
{
    fw->update([this] (std::string path_to_watch, FileStatus status) -> void 
    {
        if(!std::filesystem::is_regular_file(std::filesystem::path(path_to_watch)) && status != FileStatus::erased) 
        {
            Tools::console->AddLog("NO REGULAR: %s", path_to_watch.c_str());
            return;
        }

        switch(status) {
            case FileStatus::created:
                Tools::console->AddLog("File created: %s", path_to_watch.c_str());
                break;
            case FileStatus::modified:
                Tools::console->AddLog("File modified: %s",path_to_watch.c_str());
                if (IsFileExtension(path_to_watch.c_str(),".bas")) {
                    bios->SetFile(path_to_watch);
                    bios->ShouldRun = true;
                }
                else{
                    Tools::console->AddLog("Is not a .bas");
                }
                break;
            case FileStatus::erased:
                Tools::console->AddLog("File erased: %s", path_to_watch.c_str());
                break;
            default:
                Tools::console->AddLog("Error! Unknown file status.");
        }
    });
}

const char* Engine::GetEngineName()
{
    return TextFormat("peryEngine v: %.3f", PE_VERSION / 1000.0f);
}

