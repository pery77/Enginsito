#include "engine.h"

#include "mb_manager.h"
#include "postProcessing.h"
#include "bios.h"
#include "FileWatcher.h"
#include "editor.h"
#include "sprite_manager.h"

//Memory map
//0    - 47     Palette (Color C -> R G B) (Color 0 -> 0 1 2) (Color 1 -> 3 4 5)
//48   - 2088   Graphics (8 bytes per sprite, 256 sprites)
//2096 - 3375   MetaSprites   (20 bytes per meta, 64 metas)
//3376 - ---   Sfx (11 bytes per sfx, 16 sfx)
//--- - 4079   Unused
//4080 - 4090   CRT 4080 Blur, 4081 BlurFactor, 4082 Chromatic, 4083 Curvature, 
//                  4084 Vignetting, 4085 ScanLine, 4086 VerticalLine, 4087 GrilleForce
//                  4088 Noise, 4089 Fliker, 4090 Grille
//4091-4093 not used at moment.
//4094-4095 version (2 bytes)

FilePathList droppedFiles = { 0 };

MBManager* basicIntepreter;
PostProcessing* postProcessing;
Bios* bios;
FileWatcher* fw;
Editor* editor;
SpriteManager* spriteManager;

Engine::Engine()
{
    std::stringstream ss;
    ss << "./" << ASSETS_FOLDER << "/";
    fw = new FileWatcher{ss.str(), 3.0f};
}

Engine::~Engine()
{
    delete editor;
    basicIntepreter->CloseBas();
    delete postProcessing;
    delete basicIntepreter;
    delete bios;
    delete spriteManager;
}

void Engine::Init()
{
    LoadDefaultMemory();

    postProcessing = new PostProcessing(this);
    audioManager = new AudioManager(this);
    basicIntepreter = new MBManager(this);
    bios = new Bios(this);
    editor = new Editor(this);

    spriteManager = new SpriteManager(this);

    //DISABLED WREN AT MOMENT
    //WrenManager* wren = new WrenManager();
}

void Engine::LoadDefaultMemory()
{
    std::stringstream ss;
    ss << CONFIG_FOLDER << "/default" << MEM_EXTENSION;
    LoadMemory(ss.str().c_str());
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
    if (!FileWatcherEnabled) return;

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

unsigned char* Engine::GetMemory()
{
    return MainMemory;
}

void Engine::DumpMemory(const char *path) 
{
    FILE *f = fopen(path, "wb");
    if (f) {
        SetVersion();
        size_t r = fwrite(MainMemory, sizeof(MainMemory[0]), 4096, f);
        fclose(f);
    }
}

void Engine::LoadMemory(const char *path) 
{
    FILE *f = fopen(path, "rb");
    if (f) {
        size_t r = fread(MainMemory, sizeof(MainMemory), 1, f);
        fclose(f);
    }
}

unsigned char Engine::Peek(unsigned short dir) {
    dir = Tools::IntClamp(dir, 0, 4096);
    return (MainMemory)[dir];
}

void Engine::Poke(unsigned short dir, unsigned char value) {
    dir = Tools::IntClamp(dir, 0, 4096);
    value = Tools::IntClamp(value, 0, 255);
    MainMemory[dir] = value;
}

int Engine::GetVersion(){
    return (Peek(4094)*256) + Peek(4095);
}

void Engine::SetVersion(){
    Poke(4094, PE_VERSION / 256);
    Poke(4095, PE_VERSION % 256);
}

void Engine::SetVirtualMouse(int x,int y) 
{   
    if (isInImGui)
    {
        editor->SetMousePosInEditor(x,y);
        return;
    }
	float screenScale = Tools::Min((float)GetScreenWidth()/GAME_SCREEN_W,(float)GetScreenHeight()/GAME_SCREEN_H);
    SetMousePosition(x * screenScale, y * screenScale);
}