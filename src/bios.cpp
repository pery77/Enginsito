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
        //printf("Key: %i\n",key);
    }
    if (ch != 0 && MeasureText(currentLine.c_str(),8) < 310){
        currentLine.push_back(char(ch));
        //printf("Char: %i\n",ch);
    }
}



void Bios::ProcessCommand()
{
    Tools::Trim((char*)currentLine.c_str());
    char delim[] = " ";
    char *ptr = strtok((char*)currentLine.c_str(), delim);
    while(ptr != NULL)
	{
		printf("'%s'\n", ptr);
		ptr = strtok(NULL, delim);
	}

    currentLine.push_back('\n');
    screenLines += currentLine;
    currentLine.clear();;
}