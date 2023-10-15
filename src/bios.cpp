#include "bios.h"
#include "engine.h"
#include "postProcessing.h"
#include "sprite_manager.h"
#include "editor.h"

Engine* biosEngineRef;

Bios::Bios(Engine* _engine)
{
    biosEngineRef = _engine;
}

Bios::~Bios(){}

void Bios::LoadBoot()
{
    std::string line;
    std::ifstream myfile (BOOT_FILE);
    if (myfile.is_open()){
        while ( getline (myfile, line) )
        {
            currentLine =  line;
            ProcessCommand();
        }
        myfile.close();
    }else{
        printf("No boot file found.\n");
    }
}

void Bios::Update(){
    ClearBackground(biosEngineRef->spriteManager->GetColor(backColor));

    delta += GetFrameTime();
    cursor = (delta > 0.5) == 0 ? "_" : "";
    if (delta > 1.0) delta = 0;

    std::stringstream ss(screenLines);
    int lineY = 0;
    std::string temp;
    bool overLine = false;

    while (std::getline(ss, temp)){
        DrawTextEx(biosEngineRef->spriteManager->font, temp.c_str(), (Vector2){0, lineY}, 8, 0, biosEngineRef->spriteManager->GetColor(frontColor));
        lineY += 9;
        if (lineY > 184){   
            overLine = true;
            break;
        }
    }

    int ch = GetCharPressed();
    int key = GetKeyPressed();

    if (!overLine){
        DrawTextEx(biosEngineRef->spriteManager->font,TextFormat("%s:>%s%s",CurrentPath.c_str(), currentLine.c_str(), cursor), (Vector2){0, lineY}, 8, 0,biosEngineRef->spriteManager->GetColor(frontColor));

        if (key != 0){
            if (key == 257) { //Enter
                ProcessCommand();
                if (lineY > 171) screenLines.erase(0, screenLines.find("\n") + 1);
            }
            if (key == 259 && strlen(currentLine.c_str()) > 0) currentLine.pop_back(); //Backspace
            if (key == 261 && strlen(currentLine.c_str()) > 0) currentLine.clear(); 
            if (key == 298) printf(CurrentPath.c_str());//F9 tests
            //https://www.barcodefaq.com/ascii-chart-char-set/

        }
        if (ch != 0 && MeasureTextEx(biosEngineRef->spriteManager->font, currentLine.c_str(),8,0).x < 312){
            currentLine += Tools::GetCharFromCodepoint(ch);
        }
    }
    else{
        DrawTextEx(biosEngineRef->spriteManager->font,"Press Enter to continue.",(Vector2){0,lineY},8,0,biosEngineRef->spriteManager->GetBiosColor(frontColor));
        if (IsKeyReleased(KEY_ENTER))
            screenLines.erase(0, screenLines.find("\n") + 1);
    }

}
bool Bios::checkCommand(std::string command, std::string value){
        return std::equal(command.begin(), command.end(),
                      value.begin(), value.end(),
                      [](char command, char value) {
                          return tolower(command) == tolower(value);
                      });
}
void Bios::Print(char* line)
{
    char sline[1024];
    strcpy(sline, "print ");
    strcat(sline, line);
    currentLine = sline;
    ProcessCommand();
}

void Bios::ProcessCommand()
{
    currentLine.push_back('\n');
    screenLines += currentLine;

    int init_size = strlen(currentLine.c_str());
    char delim[] = " ";

    Tools::Trim((char*)currentLine.c_str());
    char *ptr = strtok((char*)currentLine.c_str(), delim);
    
    lastCommand.command = "";
    for (int i = 0; i < 9; i++)	{
        lastCommand.args[i] = "";
	}

    int i = 0;
    while(ptr != NULL){
        if (i == 0) 
            lastCommand.command = ptr;//Tools::ToUpper(ptr);
        else
            lastCommand.args[i-1] = ptr;//Tools::ToUpper(ptr);

        i++;
		ptr = strtok(NULL, delim);
        if (i>9) break;
	}

/*
    printf("Command: '%s'\n", lastCommand.command.c_str()); 
    for (int i = 0; i < 9; i++)	{
        if (lastCommand.args[i] == "") break;
		printf("Args: '%s', ", lastCommand.args[i].c_str()); 
	}
*/
    currentLine.clear();

    if(checkCommand(lastCommand.command,"HELP")){
        //screenLines += HelpInfo;
        screenLines += helpText;
        return;
    }

    if (checkCommand(lastCommand.command,"CLS")){
        screenLines.clear();
        return;
    }

    if (checkCommand(lastCommand.command,"EXIT")){
        ShouldClose = true;
        return;
    }

    if (checkCommand(lastCommand.command,"COLOR")){
        int bc = atoi(lastCommand.args[0].c_str());
        int fc = atoi(lastCommand.args[1].c_str());
        if (bc != fc && bc < 64 && fc < 64){
            backColor = bc;
            frontColor = fc;
        }
        return;
    }

    if (checkCommand(lastCommand.command,"RUN")){
        ShouldRun = true;
        return;
    }

    if (checkCommand(lastCommand.command,"DIR")){
        std::stringstream ss = Tools::GetFolders(CurrentPath.c_str());
        std::string temp;
        while (std::getline(ss, temp)){
            screenLines += "[" + temp + "]\n";
        }
        ss = Tools::GetFiles(CurrentPath.c_str());
        while (std::getline(ss, temp)){
            screenLines += "> " + temp + "\n";
        }
        return;
    }

    if (checkCommand(lastCommand.command,"PRINT")){
        lastCommand.args[0].push_back('\n');
        screenLines += lastCommand.args[0];
        return;
    }

    if (checkCommand(lastCommand.command,"CD.")){
        RemoveSubPath();
        
    }
    if (checkCommand(lastCommand.command,"CD..")){
        CurrentPath = "";
    }

    if (checkCommand(lastCommand.command,"CD")){
        if (lastCommand.args[0].find('/') != std::string::npos) return;
        if (lastCommand.args[0] == "" ) return;
        if (CurrentPath == ""){
            if (lastCommand.args[0].find('.') != std::string::npos) return;
            AddSubPath(lastCommand.args[0]);
        }
        else{
            if (lastCommand.args[0].find('.') != std::string::npos) {
                    RemoveSubPath();
                }
                else {
                    AddSubPath(lastCommand.args[0]);
                }
        }
        return;
    }

    if (checkCommand(lastCommand.command,"MEM")){
        if (CurrentProject.name.size() > 0)
            screenLines += CurrentProject.name + " loaded.\n";
        else
            screenLines += "No program loaded.\n";
        return;
    }

    if (checkCommand(lastCommand.command,"LOAD")){
        if (Tools::FileExist(CurrentPath, lastCommand.args[0])){
            biosEngineRef->editor->SaveCurrentFile();
            SetProgram(lastCommand.args[0]);
            biosEngineRef->editor->OpenFile();
            screenLines += "Loaded " + CurrentProject.name + " in memory.\n";
        }
        else{
            screenLines += "Fail loading " + lastCommand.args[0] + "\n";
            CurrentProject.name = "";
        }

        return;
    }

    if (checkCommand(lastCommand.command,"EDITOR"))
    {
        if (lastCommand.args[0] != "" )
        {
            biosEngineRef->editor->Enabled = (lastCommand.args[0] != "0");
        }

        std::string temp = (biosEngineRef->editor->Enabled) ? "enabled." : "disabled.";
        screenLines += "Editor is " + temp + "\n";
        
    }

    if (checkCommand(lastCommand.command,"FULLSCREEN")) biosEngineRef->postProcessing->FullScreen();
/*
    if (checkCommand(lastCommand.command,"FILEWATCHER"))
    {
        if (lastCommand.args[0] != "" )
        {
            biosEngineRef->FileWatcherEnabled = (lastCommand.args[0] != "0");
            if (biosEngineRef->FileWatcherEnabled )
            {
                screenLines += "FileWatcher is enabled.\n";
            }
            else
            {
                screenLines += "FileWatcher is disabled.\n";
            }
        }
        else 
        {
            std::string temp = (biosEngineRef->FileWatcherEnabled) ? "enabled." : "disabled.";
            screenLines += "FileWatcher is " + temp + "\n";
        }
    }
*/
}

void Bios::TryToSaveMemory()
{
    if (Tools::FileMemoryExist(CurrentProject.memoryFile))
    {
        Tools::console->AddLog("Saving current memory: [%s]", CurrentProject.memoryFile.c_str());
        biosEngineRef->DumpMemory(CurrentProject.memoryFile.c_str());
    }
}

void Bios::TryToLoadMemory()
{
    if (Tools::FileMemoryExist(CurrentProject.memoryFile))
    {
        Tools::console->AddLog("Loading data file [%s]", CurrentProject.memoryFile.c_str());
        biosEngineRef->LoadMemory(CurrentProject.memoryFile.c_str());
    } 
    else
    {
        Tools::console->AddLog("Creating data file [%s]", CurrentProject.memoryFile.c_str());
        biosEngineRef->DumpMemory(CurrentProject.memoryFile.c_str());
        biosEngineRef->LoadMemory(CurrentProject.memoryFile.c_str());
    }
}

void Bios::SetProgram(std::string file)
{
    namespace fs = std::filesystem;
    fs::path dir (ASSETS_FOLDER);
    fs::path fileP (file + PROGRAM_EXTENSION);
    fs::path fileM (file + MEM_EXTENSION);
    fs::path programFile = dir / CurrentPath / fileP;
    fs::path memoryFile = dir / CurrentPath / fileM;

    CurrentProject.name = file;
    CurrentProject.programFile = programFile.string();
    CurrentProject.memoryFile = memoryFile.string();

    TryToLoadMemory();
}

void Bios::SetFile(std::string filePath)
{
    namespace fs = std::filesystem;
    fs::path file = filePath;

    // Remove the "./assets/" prefix from the path (if present)
    std::string assetsPrefix = std::string("./") + ASSETS_FOLDER + "/";
    if (file.has_relative_path() && file.relative_path().string().substr(0, assetsPrefix.length()) == assetsPrefix)
    {
        file = file.lexically_relative(assetsPrefix);
    }

    CurrentPath = file.parent_path().string();

    std::replace(CurrentPath.begin(), CurrentPath.end(), '\\', '/');
    Tools::console->AddLog("Set file> %s\n",file.string().c_str());

    //biosEngineRef->editor->SaveCurrentFile();
    SetProgram(file.stem().string());
    biosEngineRef->editor->OpenFile();
}

void Bios::AddSubPath(std::string subPath)
{
    std::string prePath = (CurrentPath == "") ? "" : CurrentPath + "/";
     if (Tools::DirExist(prePath + subPath)) {
        if(CurrentPath == ""){
            CurrentPath = subPath;
            return;
        }
        else{
            CurrentPath += "/" + subPath;
        }
     }
}

void Bios::RemoveSubPath()
{
    std::vector<std::string> paths = Tools::Split(CurrentPath, '/');

    if (paths.empty()) return;
    if (paths.size() == 1){
        CurrentPath = "";
        return;
    }
    paths.pop_back();

    std::string tmp = "";
    
    for (int i = 0; i < paths.size(); i++){       
        if (i < paths.size() - 1)
            tmp += paths[i] + '/';
        else
            tmp += paths[i];        
    }

    CurrentPath = tmp;
}