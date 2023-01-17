#include "bios.h"

Bios::Bios(){}
Bios::~Bios(){}

void Bios::Update(){
    ClearBackground(Tools::GetColor(backColor));

    delta += GetFrameTime();
    cursor = (delta > 0.5) == 0 ? "_" : "";
    if (delta > 1.0) delta = 0;

    std::stringstream ss(screenLines);
    int lineY = 0;
    std::string temp;
    bool overLine = false;

    while (std::getline(ss, temp)){
        DrawText(temp.c_str(), 0, lineY, 8, Tools::GetColor(frontColor));
        lineY += 9;
        if (lineY > 184){   
            overLine = true;
            break;
        }
    }

    int ch = GetCharPressed();
    int key = GetKeyPressed();

    if (!overLine){
        DrawText(TextFormat("%s:>%s%s",CurrentPath.c_str(), currentLine.c_str(), cursor),0,lineY,8,Tools::GetColor(frontColor));
        if (key != 0){
            if (key == 257) { //Enter
                ProcessCommand();
                if (lineY > 171) screenLines.erase(0, screenLines.find("\n") + 1);
            }
            if (key == 259 && strlen(currentLine.c_str()) > 0) currentLine.pop_back(); //Backspace
            if (key == 261 && strlen(currentLine.c_str()) > 0) currentLine.clear(); 
            if (key == 298) printf(CurrentPath.c_str());//F9 tests
            //printf("Key: %i\n",key); 
            
        }
        if (ch != 0 && MeasureText(currentLine.c_str(),8) < 310){
            currentLine.push_back(char(ch));
            //printf("Char: %i\n",ch);
        }
    }
    else{
        DrawText("Press Enter to continue.",0,lineY,8,Tools::GetColor(frontColor));
        if (IsKeyReleased(KEY_ENTER))
            screenLines.erase(0, screenLines.find("\n") + 1);
    }

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
            lastCommand.command = Tools::ToUpper(ptr);
        else
            lastCommand.args[i-1] = Tools::ToUpper(ptr);

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
    if(lastCommand.command == "HELP"){
        screenLines += HelpInfo;
        return;
    }

    if (lastCommand.command == "CLS"){
        screenLines.clear();
        return;
    }
    if (lastCommand.command == "EXIT"){
        ShouldClose = true;
        return;
    }
    if (lastCommand.command == "COLOR"){
        int bc = atoi(lastCommand.args[0].c_str());
        int fc = atoi(lastCommand.args[1].c_str());
        if (bc != fc && bc < 64 && fc < 64){
            backColor = bc;
            frontColor = fc;
        }
        return;
    }
    if (lastCommand.command == "RUN"){
        ShouldRun = true;
        return;
    }
    if (lastCommand.command == "LIST"){
        std::stringstream ss = Tools::GetFiles(CurrentPath.c_str());
        std::string temp;
        while (std::getline(ss, temp)){
            temp.push_back('\n');
            screenLines += temp.c_str();
        }

        return;
    }
    if (lastCommand.command == "PRINT"){
        lastCommand.args[0].push_back('\n');
        screenLines += lastCommand.args[0];
        return;
    }
    if (lastCommand.command == "CD"){
        
        if (lastCommand.args[0].find('.') != std::string::npos && CurrentPath == ""){
            return;
        }
        if (Tools::DirExist(lastCommand.args[0].c_str()))
            CurrentPath = lastCommand.args[0];
        return;
    }
    if (lastCommand.command == "MEM"){
        if (CurrentProgram != "")
            screenLines += CurrentProgram + " loaded.\n";
        else
            screenLines += "No program loaded.\n";
        return;
    }
    if (lastCommand.command == "LOAD"){
        if (Tools::FileExist(CurrentPath, lastCommand.args[0])){
            CurrentProgram = lastCommand.args[0];
            screenLines += "Loaded " + CurrentProgram + " in memory.\n";

        }
        else{
            screenLines += "Fail loading " + CurrentProgram + "\n";
            CurrentProgram = "";
        }

        return;
    }
}

std::string Bios::GetFile(){
    return "assets" + CurrentPath + "/" + CurrentProgram + PROGRAM_EXTENSION;
}