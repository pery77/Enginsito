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
    while (std::getline(ss, temp)){
        DrawText(temp.c_str(), 0, lineY, 8, Tools::GetColor(frontColor));
        lineY += 9;  
    }

    DrawText(TextFormat(">%s%s",currentLine.c_str(), cursor),0,lineY,8,Tools::GetColor(frontColor));

    int ch = GetCharPressed();
    int key = GetKeyPressed();

    if (key != 0){
        if (key == 257) { //Enter
            if (lineY > 180) screenLines.erase(0, screenLines.find("\n") + 1);
            ProcessCommand();
        }
        if (key == 259 && strlen(currentLine.c_str()) > 0) currentLine.pop_back(); //Backspace
        if (key == 261 && strlen(currentLine.c_str()) > 0) currentLine.clear(); //Delete
        printf("Key: %i\n",key);
    }
    if (ch != 0 && MeasureText(currentLine.c_str(),8) < 310){
        currentLine.push_back(char(ch));
        //printf("Char: %i\n",ch);
    }
}

char* toUpper(char* s) {
  for(char *p=s; *p; p++) *p=toupper(*p);
  return s;
}

void Bios::ProcessCommand()
{
    currentLine.push_back('\n');
    screenLines += currentLine;

    int init_size = strlen(currentLine.c_str());
    char delim[] = " ";

    Tools::Trim((char*)currentLine.c_str());
    char *ptr = strtok((char*)currentLine.c_str(), delim);
    
    LastCommand.command = "";
    for (int i = 0; i < 9; i++)	{
        LastCommand.args[i] = "";
	}

    int i = 0;
    while(ptr != NULL){
        if (i == 0) 
            LastCommand.command = toUpper(ptr);
        else
            LastCommand.args[i-1] = toUpper(ptr);

        i++;
		ptr = strtok(NULL, delim);
        if (i>9) break;
	}
/*
    printf("Command: '%s'\n", LastCommand.command.c_str()); 
    for (int i = 0; i < 9; i++)	{
        if (LastCommand.args[i] == "") break;
		printf("Args: '%s', ", LastCommand.args[i].c_str()); 
	}
*/
    currentLine.clear();


    if (LastCommand.command == "CLS"){
        screenLines.clear();
        return;
    }
    if (LastCommand.command == "EXIT"){
        ShouldClose = true;
        return;
    }
    if (LastCommand.command == "COLOR"){
        int bc = atoi(LastCommand.args[0].c_str());
        int fc = atoi(LastCommand.args[1].c_str());
        if (bc != fc){
            backColor = bc;
            frontColor = fc;
        }
        return;
    }



}