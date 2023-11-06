#pragma once
#include <string>
#include "TextEditor.h"

class Documentation
{
private:
    void Keyword(const char* name, const char* args, const char* program = "");
public:
    std::string ProgramName = "";
    Documentation();
    ~Documentation();
    TextEditor codeEditor;
    void Draw(bool* p_open);
};