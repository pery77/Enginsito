#pragma once
#include <string>

class Documentation
{
private:
    void Keyword(const char* name, const char* args, const char* program = "");
public:
    std::string ProgramPath = "";
    Documentation();
    ~Documentation();

    void Draw(bool* p_open);
};