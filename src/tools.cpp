#include "tools.h"

Color palette[16]{
{32,32,38,255},
{58,54,61,255},
{105,60,54,255},
{171,55,55,255},
{80,82,115,255},
{110,102,96,255},
{158,72,145,255},
{87,148,96,255},
{89,144,158,255},
{224,134,74,255},
{245,108,119,255},
{150,212,91,255},
{186,177,151,255},
{107,224,191,255},
{245,214,120,255},
{249,255,242,255}
};
Color userPalette[16]{};

int Tools::IntClamp(int value, int min, int max){
    if (value < min) return min;
    if (value > max) return max;
}
Color Tools::GetColor(int color){
    return userPalette[IntClamp(color, 0, 15)];
}
Color Tools::GetFixedColor(int color){
    return palette[IntClamp(color, 0, 15)];
}
Color Tools::SetColor(int color, int r, int g, int b){
    r = IntClamp(r, 0, 255);
    g = IntClamp(g, 0, 255);
    b = IntClamp(b, 0, 255);
    userPalette[IntClamp(color, 0, 15)] = {(unsigned char)r,(unsigned char)g,(unsigned char)b,255};
}
void Tools::CopyPalette(){
    for (int i = 0; i < 16; ++i){
        userPalette[i] = palette[i];
    } 
}


int Tools::GetVirtualMouse(bool isXAxis){   
	float screenScale = min((float)GetScreenWidth()/GAME_SCREEN_W,(float)GetScreenHeight()/GAME_SCREEN_H);
	float mouse = isXAxis ? GetMousePosition().x : GetMousePosition().y;
    float screen = isXAxis ? GetScreenWidth() : GetScreenHeight();
    float gamescreen = isXAxis ? GAME_SCREEN_W : GAME_SCREEN_H;
    float value = (mouse - (screen - (gamescreen * screenScale)) * 0.5f) / screenScale;
    return (int)Clamp(value, 0,(float)gamescreen);
}

bool Tools::CompareFloats(float x, float y, float epsilon){
   if(fabs(x - y) < epsilon) return true;
      return false;
}

void Tools::Trim(char * s) {
    char * p = s;
    int l = strlen(p);

    while(isspace(p[l - 1])) p[--l] = 0;
    while(* p && isspace(* p)) ++p, --l;

    memmove(s, p, l + 1);
}

char* Tools::ToUpper(char* s) {
  for(char *p=s; *p; p++) *p=toupper(*p);
  return s;
}
std::vector<std::string> Tools::Split(const std::string& str, const char sep)
{
    std::string token; 
    std::stringstream ss(str);
    std::vector<std::string> tokens;
    
    while (std::getline(ss, token, sep)) {
        tokens.push_back(token);
    }
    
    return tokens;
}
std::stringstream Tools::GetFiles(const char *path) {
    
    namespace fs = std::filesystem;
    const fs::path current_path = fs::current_path() / ASSETS_FOLDER / path;
    std::stringstream result;

    DIR *dir = opendir(current_path.string().c_str());
   
    if (dir == NULL) {
        return result;
    }

    for(auto& p : std::filesystem::directory_iterator(current_path)){
        if (p.is_directory()){
            std::string folder = p.path().filename().string();
            result << " < " << folder << " > " << "\n";
        }
    }

    for(auto& p : std::filesystem::directory_iterator(current_path)){
        if (p.is_regular_file()){
            std::string file = p.path().stem().string();
            std::string ext = p.path().extension().string();
            ext = ToUpper((char*)ext.c_str());
            if (ext == PROGRAM_EXTENSION)
                result <<  "  |· " << file << "\n";
        }
    }
            
    closedir(dir);
    return result;
}
bool Tools::DirExist(std::string path){
    namespace fs = std::filesystem;
    const fs::path current_path = fs::current_path() / ASSETS_FOLDER / path;
    bool result = fs::exists(current_path);
    printf("[%s] Dir> %s\n",result ? "OK" : "Fail", current_path.string().c_str());
    return result;
}

bool Tools::FileExist(std::string path, std::string file){
    namespace fs = std::filesystem;
    file += PROGRAM_EXTENSION;
    const fs::path current_path = fs::current_path() / ASSETS_FOLDER / path / file;
    bool result = fs::exists(current_path);
    printf("[%s] File> %s\n",result ? "OK" : "Fail", current_path.string().c_str());
    return result;
}