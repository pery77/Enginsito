#include "bios.h"


Bios::Bios(){
}
Bios::~Bios(){
}


void Bios::Update(){
    ClearBackground(BLACK);

    delta += GetFrameTime();
    cursor = (delta > 0.5) == 0 ? "_" : "";
    if (delta > 1.0) delta = 0;


    int lineY = 0;
    std::string temp; 

    for (char& c : screenLines){
        DrawText(screenLines.c_str(), 0, lineY, 8, WHITE);
        if (c == '\n') lineY += 8;
    }
    DrawText(TextFormat(">%s%s",currentLine.c_str(), cursor),0,lineY,8,WHITE);

    int ch = GetCharPressed();
    int key = GetKeyPressed();

    if (key != 0){
        //printf("Key: %i\n",key);
        if (key == 257) ProcessCommand();
        if (key == 259 && strlen(currentLine.c_str()) > 0) currentLine.pop_back();
    }
    if (ch != 0){
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