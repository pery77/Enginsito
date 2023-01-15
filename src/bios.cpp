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
        //printf("Key: %i\n",key);
        if (key == 257) {
            ProcessCommand();
            printf("liney: %i\n", lineY);
            if (lineY > 180) screenLines.erase(0, screenLines.find("\n") + 1);
        }
        if (key == 259 && strlen(currentLine.c_str()) > 0) currentLine.pop_back();
    }
    if (ch != 0 && MeasureText(currentLine.c_str(),8) < 310){
        //printf("Char: %i\n",ch);
        currentLine.push_back(char(ch));
    }
}

void Bios::ProcessCommand()
{
    currentLine.push_back('\n');
    screenLines += currentLine;
    printf("%s", screenLines.c_str());
    currentLine = "";
}