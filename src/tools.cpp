#include "tools.h"

Color palette[] = {
{19,19,19,255},{27,27,27,255},{39,39,39,255},
{61,61,61,255},{93,93,93,255},{133,133,133,255},{180,180,180,255},
{255,255,255,255},{199,207,221,255},{146,161,185,255},{101,115,146,255},
{66,76,110,255},{42,47,78,255},{26,25,50,255},{14,7,27,255},
{28,18,28,255},{57,31,33,255},{93,44,40,255},{138,72,54,255},
{191,111,74,255},{230,156,105,255},{246,202,159,255},{249,230,207,255},
{237,171,80,255},{224,116,56,255},{198,69,36,255},{142,37,29,255},
{255,80,0,255},{237,118,20,255},{255,162,20,255},{255,200,37,255},
{255,235,87,255},{211,252,126,255},{153,230,95,255},{90,197,79,255},
{51,152,75,255},{30,111,80,255},{19,76,76,255},{12,46,68,255},
{0,57,109,255},{0,105,170,255},{0,152,220,255},{0,205,249,255},
{12,241,255,255},{148,253,255,255},{253,210,237,255},{243,137,245,255},
{219,63,253,255},{122,9,250,255},{48,3,217,255},{12,2,147,255},
{3,25,63,255},{59,20,67,255},{98,36,97,255},{147,56,143,255},
{202,82,201,255},{200,80,134,255},{246,129,135,255},{245,85,93,255},
{234,50,60,255},{196,36,48,255},{137,30,43,255},{87,28,39,255},{255,0,64,255},{0,0,0,0}
};

Color Tools::GetColor(int id){
    if (id < 0 ) id = 64;
    if (id > 63) id = 64;
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