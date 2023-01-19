#include "tools.h"

Color palette[] = {
{8,8,8,255},
{34,32,52,255},
{69,40,60,255},
{102,57,49,255},
{143,86,59,255},
{223,113,38,255},
{217,160,102,255},
{238,195,154,255},
{251,242,54,255},
{153,229,80,255},
{106,190,48,255},
{55,148,110,255},
{75,105,47,255},
{82,75,36,255},
{50,60,57,255},
{63,63,116,255},
{48,96,130,255},
{91,110,225,255},
{99,155,255,255},
{95,205,228,255},
{203,219,252,255},
{255,255,255,255},
{155,173,183,255},
{132,126,135,255},
{105,106,106,255},
{89,86,82,255},
{118,66,138,255},
{172,50,50,255},
{217,87,99,255},
{215,123,186,255},
{143,151,74,255},
{138,111,48,255}
};

Color Tools::GetColor(int id){
    if (id < 0 ) id = 0;
    if (id > 31) id = 32;
    return palette[id];
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