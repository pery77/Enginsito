#include "engine.h"

#include "mb_manager.h"
#include "postProcessing.h"
#include "bios.h"
//#include "FileWatcher.h"
#include "editor.h"
#include "sprite_manager.h"

//Memory map
//0    - 47     Palette (Color C -> R G B) (Color 0 -> 0 1 2) (Color 1 -> 3 4 5)
//48   - 2088   Graphics (8 bytes per sprite, 256 sprites)
//2096 - 3375   MetaSprites   (20 bytes per meta, 64 metas)
//3376 - 3551   Sfx (11 bytes per sfx, 16 sfx)
//3552 - 3583   Save data 2 bytes 0 -  15 slots
//3584 - 3647   Save data 4 bytes 16 - 31 slots
//3648 - 4079   Unused 
//3648 - 4079   Unused 
//4080 - 4090   CRT 4080 Blur, 4081 BlurFactor, 4082 Chromatic, 4083 Curvature, 
//                  4084 Vignetting, 4085 ScanLine, 4086 VerticalLine, 4087 GrilleForce
//                  4088 Noise, 4089 Fliker, 4090 Grille
//4091-4093 not used at moment.
//4094-4095 version (2 bytes)

FilePathList droppedFiles = { 0 };

MBManager* basicIntepreter;
PostProcessing* postProcessing;
Bios* bios;
//FileWatcher* fw;
Editor* editor;
SpriteManager* spriteManager;

Engine::Engine()
{
    //std::stringstream ss;
    //ss << "./" << ASSETS_FOLDER << "/";
    //fw = new FileWatcher{ss.str(), 3.0f};
}

Engine::~Engine()
{
    basicIntepreter->CloseBas();
    delete editor;
    delete postProcessing;
    delete basicIntepreter;
    delete bios;
    delete spriteManager;
}

void Engine::Init()
{
    postProcessing = new PostProcessing(this);
    audioManager = new AudioManager(this);
    basicIntepreter = new MBManager(this);
    bios = new Bios(this);
    editor = new Editor(this);

    spriteManager = new SpriteManager(this);

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

        const char* fileExtension = strrchr(firstFilePath, '.');
        if (strcmp(fileExtension, ".hex") == 0)
        {
            ReadHexFile(firstFilePath);
            return;
        }

        if (strcmp(fileExtension, ".crtPreset") == 0)
        {
            LoadMemory(firstFilePath, 4080, 11);
            return;
        }
#ifdef DEBUG
        if (strcmp(fileExtension, ".data") == 0)
        {
            std::ifstream input(firstFilePath, std::ios::binary);
            std::ofstream output("data.txt",  std::ios::binary);
            std::vector<unsigned char> buffer(std::istreambuf_iterator<char>(input), {});
            input.close();
            output << "static unsigned char FILE_DATA[" << buffer.size() << "] = { ";
            for (std::size_t i = 0; i < buffer.size(); ++i) {
                if (i > 0) {
                    output << ", ";
                }
                output << "0x" << std::hex << static_cast<int>(buffer[i]);
            }
            output << " };" << std::endl;

            output.close();

        }
#endif        
    }
}
void Engine::ReadHexFile(const char* filePath)
{
    FILE* file = fopen(filePath, "r");
    if (file == nullptr)
    {
        Tools::console->AddLog("[ERROR] Failed to open .hex file: %s\n", filePath);
        return;
    }

    char line[7];
    int id = 0;

    while (fgets(line, sizeof(line), file) != nullptr && id<16)
    {
        unsigned int colorValue;
        if (sscanf(line, "%x", &colorValue) != 1) continue;

        unsigned char r = (colorValue >> 16) & 0xFF;
        unsigned char g = (colorValue >> 8) & 0xFF;
        unsigned char b = colorValue & 0xFF;

        int dir = id * 3;
        Poke(dir, r);
        Poke(dir + 1, g);
        Poke(dir + 2, b);

        Tools::console->AddLog("[INFO] Color %i is: %s", id, line);
        id++;
    }

    fclose(file);
}
/*
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
                    bios->TryToSaveMemory();
                    bios->SetFile(path_to_watch);
                    bios->ShouldRun = true;
                    postProcessing->Refresh();
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
*/
const char* Engine::GetEngineName()
{
    return TextFormat("Enginsito v: %.3f", PE_VERSION / 1000.0f);
}

unsigned char* Engine::GetMemory()
{
    return MainMemory;
}

void Engine::DumpMemory(const char *path, unsigned short start, unsigned short total) 
{
    FILE *f = fopen(path, "wb");
    if (f) {
        SetVersion();
        if (start + total <= 4096) {
            size_t r = fwrite(&MainMemory[start], 1, total, f);
        }
        fclose(f);
    }
}

void Engine::LoadMemory(const char *path, int start, int total) 
{
    FILE *f = fopen(path, "rb");
    if (f) {
        //size_t r = fread(MainMemory, sizeof(MainMemory), 1, f);
        if (start + total <= 4096) {
            fseek(f, start, SEEK_SET);
            size_t r = fread(&MainMemory[start], 1, total, f);
            Tools::console->AddLog("Loading %i bytes", r);
            Tools::console->AddLog("From %i to %i", start, start + total);
        }
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