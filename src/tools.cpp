#include "tools.h"

Console* Tools::console = new Console();
static const char* layouts[5] = {"config/layout_1.ini", "config/layout_2.ini", "config/layout_3.ini", "config/layout_4.ini", "config/layout_5.ini"};

int Tools::IntClamp(int value, int min, int max)
{
    if (value < min) return min;
    if (value > max) return max;
}

int Tools::GetVirtualMouse(bool isXAxis) 
{   
	float screenScale = Min((float)GetScreenWidth()/GAME_SCREEN_W,(float)GetScreenHeight()/GAME_SCREEN_H);
	float mouse = isXAxis ? GetMousePosition().x : GetMousePosition().y;
    float screen = isXAxis ? GetScreenWidth() : GetScreenHeight();
    float gamescreen = isXAxis ? GAME_SCREEN_W : GAME_SCREEN_H;
    float value = std::ceil((mouse - (screen - (gamescreen * screenScale)) * 0.5f) / screenScale);
    return (int)Clamp(value, 0,(float)gamescreen);
}

bool Tools::CompareFloats(float x, float y, float epsilon) 
{
   if(fabs(x - y) < epsilon) return true;
      return false;
}

void Tools::Trim(char * s) 
{
    char * p = s;
    int l = strlen(p);

    while(isspace(p[l - 1])) p[--l] = 0;
    while(* p && isspace(* p)) ++p, --l;

    memmove(s, p, l + 1);
}

char* Tools::ToUpper(char* s) 
{
  for(char *p=s; *p; p++) *p=toupper(*p);
  return s;
}

std::string Tools::GetCharFromCodepoint(int codepoint) 
{
    static char utf8[] = { 0 };
    if (codepoint >= 32 && codepoint <= 127){
        utf8[0] = (char)codepoint;
        utf8[1] = '\0';
        char ch = (char)codepoint;
        std::string str = "";
        return str + ch;
    }
    else if  (codepoint >= 128 && codepoint <= 256)
    {
        utf8[0] = (char)(((codepoint >> 6) & 0x1f) | 0xc0);
        utf8[1] = (char)((codepoint & 0x3f) | 0x80);  
        utf8[2] = '\0';
        std::string str(utf8);
        return str;
    }

    return "<?>";
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

std::stringstream Tools::GetFiles(const char *path) 
{
    std::stringstream result;
    if(!CheckAssetsFolder()) return result;

    namespace fs = std::filesystem;
    const fs::path current_path = fs::current_path() / ASSETS_FOLDER / path;

    DIR *dir = opendir(current_path.string().c_str());
   
    if (dir == NULL) {
        return result;
    }

    for(auto& p : std::filesystem::directory_iterator(current_path)){
        if (p.is_regular_file()) {
            std::string file = p.path().stem().string();   
            if (IsFileExtension(p.path().filename().string().c_str(), PROGRAM_EXTENSION))
                result  << file << "\n";
        }
    }
            
    closedir(dir);
    return result;
}
std::stringstream Tools::GetFolders(const char *path) 
{
    std::stringstream result;
    if(!CheckAssetsFolder()) return result;

    namespace fs = std::filesystem;
    const fs::path current_path = fs::current_path() / ASSETS_FOLDER / path;

    DIR *dir = opendir(current_path.string().c_str());
   
    if (dir == NULL) {
        return result;
    }

    for(auto& p : std::filesystem::directory_iterator(current_path)) {
        if (p.is_directory()) {
            std::string folder = p.path().filename().string();
            result << folder << "\n";
        }
    }
        
    closedir(dir);
    return result;
}

std::stringstream Tools::GetDir(const char *path) 
{
    std::stringstream folders = GetFolders(path);
    std::stringstream files = GetFiles(path);
    std::stringstream result;
    result << folders.str() << files.str();
    return result;
}
bool Tools::CheckAssetsFolder() 
{
    namespace fs = std::filesystem;
    const fs::path current_path = fs::current_path() / ASSETS_FOLDER;
    bool result = fs::exists(current_path);
    if (!result)
        printf("No /assets folder found.\n");

    return result;
}

bool Tools::DirExist(std::string path) 
{
    namespace fs = std::filesystem;
    const fs::path current_path = fs::current_path() / ASSETS_FOLDER / path;
    bool result = fs::exists(current_path);
    printf("[%s] Dir> %s\n",result ? "OK" : "Fail", current_path.string().c_str());
    return result;
}

bool Tools::FileExist(std::string path, std::string file) 
{
    namespace fs = std::filesystem;
    file += PROGRAM_EXTENSION;
    const fs::path current_path = fs::current_path() / ASSETS_FOLDER / path / file;
    bool result = fs::exists(current_path);
    printf("[%s] File> %s\n",result ? "OK" : "Fail", current_path.string().c_str());
    return result;
}
bool Tools::FileMemoryExist(std::string memoryFile) 
{
    namespace fs = std::filesystem;
    const fs::path current_path = fs::current_path() / memoryFile;
    bool result = fs::exists(current_path);
    printf("[%s] File Memmory> %s\n",result ? "OK" : "Fail", current_path.string().c_str());
    return result;
}

float Tools::Min(float a, float b) 
{
    return a<b ? a : b;
}

int8_t Tools::ToSigned(unsigned char value)
{
    int8_t result = value;
    if (value>127) result = value - 256;
    return result;
}

unsigned char Tools::ToUnsigned(int8_t value)
{
    unsigned char result = value;
    if (value<0) result = value + 256;
    return result;
}

Texture Tools::TextureFromCode(int format, int height, int width, void* data, int mips)
{
    Image img = {0};
    img.format = format;
    img.height = height;
    img.width = width;
    img.data = data;
    img.mipmaps = mips;

    return LoadTextureFromImage(img);
}

const char* Tools::GetCurrentLayout(int currentLayout)
{
    return layouts[currentLayout];
}