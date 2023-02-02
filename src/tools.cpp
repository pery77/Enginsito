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











FontDataBytes fontData[223]{
    {48,120,120,48,48,0,48,0},          // 33 !
    {108,108,108,0,0,0,0,0},            // 34 "
    {108,108,254,108,254,108,108,0},    // 35 #
    {48,124,192,120,12,248,48,0},       // 36 $
    {0,198,204,24,48,102,198,0},        // 37 %
    {56,108,56,118,220,204,118,0},      // 38 &
    {96,96,192,0,0,0,0,0},              // 39 '
    {24,48,96,96,96,48,24,0},           // 40 (
    {96,48,24,24,24,48,96,0},           // 41 )
    {0,102,60,255,60,102,0,0},          // 42 *
    {0,48,48,252,48,48,0,0},            // 43 +
    {0,0,0,0,0,112,48,96},              // 44 ,
    {0,0,0,252,0,0,0,0},                // 45 -
    {0,0,0,0,0,48,48,0},                // 46 .
    {3,7,14,28,56,112,224,192},         // 47 /
    {120,204,220,252,236,204,120,0},    // 48 0
    {48,112,48,48,48,48,252,0},         // 49 1
    {120,204,12,56,96,204,252,0},       // 50 2
    {120,204,12,56,12,204,120,0},       // 51 3
    {28,60,108,204,254,12,12,0},        // 52 4
    {252,192,248,12,12,204,120,0},      // 53 5
    {56,96,192,248,204,204,120,0},      // 54 6
    {252,204,12,24,48,96,96,0},         // 55 7
    {120,204,204,120,204,204,120,0},    // 56 8
    {120,204,204,124,12,24,112,0},      // 57 9 
    {0,0,48,48,0,48,48,0},              // 58 :
    {0,0,48,48,0,112,48,96},            // 59 ;
    {24,48,96,192,96,48,24,0},          // 60 <
    {0,0,252,0,252,0,0,0},              // 61 =
    {96,48,24,12,24,48,96,0},           // 62 >
    {120,204,12,24,48,0,48,0},          // 63 ?
    {124,198,218,214,222,192,124,0},    // 64 @
    {48,120,204,204,252,204,204,0},     // 65 A
    {252,102,102,124,102,102,252,0},    // 66 B
    {60,102,192,192,192,102,60,0},      // 67 C
    {252,108,102,102,102,108,252,0},    // 68 D
    {254,98,104,120,104,98,254,0},      // 69 E
    {254,98,104,120,104,96,240,0},      // 70 F
    {60,102,192,206,198,102,62,0},      // 71 G
    {204,204,204,252,204,204,204,0},    // 72 H
    {120,48,48,48,48,48,120,0},         // 73 I
    {30,12,12,12,204,204,56,0},         // 74 J
    {230,102,108,120,108,102,226,0},    // 75 K
    {240,96,96,96,98,102,254,0},        // 76 L
    {198,238,254,214,198,198,198,0},    // 77 M
    {198,230,246,222,206,198,198,0},    // 78 N
    {56,108,198,198,198,108,56,0},      // 79 O
    {252,102,102,124,96,96,224,0},      // 80 P
    {120,204,204,204,220,120,28,0},     // 81 Q
    {252,102,102,124,120,108,230,0},    // 82 R
    {120,204,224,56,28,204,120,0},      // 83 S
    {252,180,48,48,48,48,120,0},        // 84 T
    {204,204,204,204,204,204,252,0},    // 85 U
    {204,204,204,204,204,120,48,0},     // 86 V
    {198,198,198,214,254,238,198,0},    // 87 W
    {198,198,108,56,108,198,198,0},     // 88 X
    {204,204,204,120,48,48,120,0},      // 89 Y
    {254,204,152,48,98,198,254,0},      // 90 Z
    {120,96,96,96,96,96,120,0},         // 91 [
    {192,224,112,56,28,14,7,3},         // 92 /<
    {120,24,24,24,24,24,120,0},         // 93 ]
    {16,56,108,198,0,0,0,0},            // 94 ^
    {0,0,0,0,0,0,0,255},                // 95 _
    {48,48,24,0,0,0,0,0},               // 96 `
    {0,0,120,12,124,204,118,0},         // 97 a
    {224,96,124,102,102,102,188,0},     // 98 b
    {0,0,120,204,192,204,120,0},        // 99 c
    {28,12,12,124,204,204,118,0},       // 100 d
    {0,0,120,204,252,192,120,0},        // 101 e
    {56,108,96,240,96,96,240,0},        // 102 f
    {0,0,118,204,204,124,12,248},       // 103 g
    {224,96,108,118,102,102,230,0},     // 104 h
    {48,0,112,48,48,48,120,0},          // 105 i
    {24,0,120,24,24,24,216,112},        // 106 j
    {224,96,102,108,120,108,230,0},     // 107 k
    {112,48,48,48,48,48,120,0},         // 108 l
    {0,0,236,254,214,198,198,0},        // 109 m
    {0,0,248,204,204,204,204,0},        // 110 n
    {0,0,120,204,204,204,120,0},        // 111 o
    {0,0,220,102,102,124,96,240},       // 112 p
    {0,0,118,204,204,124,12,30},        // 113 q
    {0,0,216,108,108,96,240,0},         // 114 r
    {0,0,124,192,120,12,248,0},         // 115 s
    {16,48,124,48,48,52,24,0},          // 116 t
    {0,0,204,204,204,204,118,0},        // 117 u
    {0,0,204,204,204,120,48,0},         // 118 v
    {0,0,198,198,214,254,108,0},        // 119 w
    {0,0,198,108,56,108,198,0},         // 120 x
    {0,0,204,204,204,124,12,248},       // 121 y
    {0,0,252,152,48,100,252,0},         // 122 z
    {28,48,48,224,48,48,28,0},          // 123 {
    {24,24,24,0,24,24,24,0},            // 124 |
    {224,48,48,28,48,48,32,224},        // 125 }
    {118,220,0,0,0,0,0,0},              // 126 ~ 
    {0,16,40,68,68,124,0,0},            // 127 DEL
    {120,204,192,204,120,24,112,0},     // 128 Ç
    {255,129,129,129,129,129,129,255},  // 
    {255,129,129,129,129,129,129,255},  // 
    {255,129,129,129,129,129,129,255},  // 
    {255,129,129,129,129,129,129,255},  // 
    {255,129,129,129,129,129,129,255},  // 
    {255,129,129,129,129,129,129,255},  // 
    {255,129,129,129,129,129,129,255},  // 
    {255,129,129,129,129,129,129,255},  // 
    {255,129,129,129,129,129,129,255},  // 
    {255,129,129,129,129,129,129,255},  // 
    {255,129,129,129,129,129,129,255},  // 
    {255,129,129,129,129,129,129,255},  // 
    {255,129,129,129,129,129,129,255},  // 
    {255,129,129,129,129,129,129,255},  // 
    {255,129,129,129,129,129,129,255},
    {255,129,129,129,129,129,129,255},  // 
    {255,129,129,129,129,129,129,255},  // 
    {255,129,129,129,129,129,129,255},  // 
    {255,129,129,129,129,129,129,255},  // 
    {255,129,129,129,129,129,129,255},  // 
    {255,129,129,129,129,129,129,255},  // 
    {255,129,129,129,129,129,129,255},  // 
    {255,129,129,129,129,129,129,255},  // 
    {255,129,129,129,129,129,129,255},  // 
    {255,129,129,129,129,129,129,255},  // 
    {255,129,129,129,129,129,129,255},  // 
    {255,129,129,129,129,129,129,255},  // 
    {255,129,129,129,129,129,129,255},  // 
    {255,129,129,129,129,129,129,255},  // 
    {255,129,129,129,129,129,129,255},  // 
    {255,129,129,129,129,129,129,255},
    {255,129,129,129,129,129,129,255},  // 
    {255,129,129,129,129,129,129,255},  // 
    {255,129,129,129,129,129,129,255},  // 
    {255,129,129,129,129,129,129,255},  // 
    {255,129,129,129,129,129,129,255},  // 
    {120,0,196,164,148,140,132,0},      // ...
    {255,129,129,129,129,129,129,255},  // 
    {255,129,129,129,129,129,129,255},  // 
    {255,129,129,129,129,129,129,255},  // 
    {60,66,157,161,161,157,66,60},      // 169 ©
    {255,129,129,129,129,129,129,255},  // 
    {255,129,129,129,129,129,129,255},  // 
    {255,129,129,129,129,129,129,255},  // 
    {255,129,129,129,129,129,129,255},  // 
    {255,129,129,129,129,129,129,255},  // 
    {255,129,129,129,129,129,129,255},
    {255,129,129,129,129,129,129,255},  // 
    {255,129,129,129,129,129,129,255},  // 
    {255,129,129,129,129,129,129,255},  // 
    {255,129,129,129,129,129,129,255},  // 
    {255,129,129,129,129,129,129,255},  // 
    {255,129,129,129,129,129,129,255},  // 
    {255,129,129,129,129,129,129,255},  // 
    {0,0,24,60,60,24,0,0},              // 183 ·
    {255,129,129,129,129,129,129,255},  // 
    {255,129,129,129,129,129,129,255},  // 
    {255,129,129,129,129,129,129,255},  // 
    {255,129,129,129,129,129,129,255},  // 
    {255,129,129,129,129,129,129,255},  // 
    {255,129,129,129,129,129,129,255},  // 
    {255,129,129,129,129,129,129,255},  // 
    {255,129,129,129,129,129,129,255},  // 
    {255,129,129,129,129,129,129,255},  // 
    {255,129,129,129,129,129,129,255},  // 
    {255,129,129,129,129,129,129,255},  // 
    {255,129,129,129,129,129,129,255},  // 
    {255,129,129,129,129,129,129,255},  // 
    {255,129,129,129,129,129,129,255},  // 
    {255,129,129,129,129,129,129,255},  // 
    {255,129,129,129,129,129,129,255},  // 
    {255,129,129,129,129,129,129,255},  // 
    {255,129,129,129,129,129,129,255},  // 
    {255,129,129,129,129,129,129,255},  // 
    {255,129,129,129,129,129,129,255},  // 
    {255,129,129,129,129,129,129,255},  // 
    {255,129,129,129,129,129,129,255},  // 
    {255,129,129,129,129,129,129,255},  // 
    {255,129,129,129,129,129,129,255},  // 
    {255,129,129,129,129,129,129,255},  // 
    {118,220,0,230,246,222,206,0},      // 209 Ñ
    {255,129,129,129,129,129,129,255},  // 
    {255,129,129,129,129,129,129,255},  // 
    {255,129,129,129,129,129,129,255},  // 
    {255,129,129,129,129,129,129,255},
    {255,129,129,129,129,129,129,255},  // 
    {255,129,129,129,129,129,129,255},  // 
    {255,129,129,129,129,129,129,255},  // 
    {255,129,129,129,129,129,129,255},  // 
    {255,129,129,129,129,129,129,255},  // 
    {255,129,129,129,129,129,129,255},  // 
    {255,129,129,129,129,129,129,255},  // 
    {255,129,129,129,129,129,129,255},  // 
    {255,129,129,129,129,129,129,255},  // 
    {255,129,129,129,129,129,129,255},  // 
    {255,129,129,129,129,129,129,255},  // 
    {255,129,129,129,129,129,129,255},  // 
    {255,129,129,129,129,129,129,255},  // 
    {255,129,129,129,129,129,129,255},  // 
    {255,129,129,129,129,129,129,255},  // 
    {255,129,129,129,129,129,129,255},
    {255,129,129,129,129,129,129,255},  // 
    {255,129,129,129,129,129,129,255},  // 
    {255,129,129,129,129,129,129,255},  // 
    {255,129,129,129,129,129,129,255},  // 
    {255,129,129,129,129,129,129,255},  // 
    {255,129,129,129,129,129,129,255},  // 
    {255,129,129,129,129,129,129,255},  // 
    {255,129,129,129,129,129,129,255},  // 
    {255,129,129,129,129,129,129,255},  // 
    {255,129,129,129,129,129,129,255},  // 
    {255,129,129,129,129,129,129,255},  // 
    {118,220,0,248,204,204,204,0},      // 241 ñ
    {96,48,0,120,204,204,120,0},        // 242 ò
    {255,129,129,129,129,129,129,255},  // 
    {255,129,129,129,129,129,129,255},  // 
    {255,129,129,129,129,129,129,255},
    {255,129,129,129,129,129,129,255},  // 
    {255,129,129,129,129,129,129,255},  // 
    {255,129,129,129,129,129,129,255},  // 
    {255,129,129,129,129,129,129,255},  // 
    {255,129,129,129,129,129,129,255},  // 
    {255,129,129,129,129,129,129,255},  // 
    {255,129,129,129,129,129,129,255},  // 
    {255,129,129,129,129,129,129,255},  // 
    {0,126,126,126,126,126,126,0},      // 254 ■
    {255,129,129,129,129,129,129,255}   // 
};

Font font = {0};
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

void Tools::SetFont(){

    #define BIT_CHECK(a,b) ((a) & (1 << (b)))

    font.glyphCount = 223;   // Number of chars included in our default font
    font.glyphPadding = 0;   // Characters padding
    int charsHeight = 8;
    int charsWidth = 8;
    
    Image imFont = {
        .data = calloc(128*128, 2),  // 2 bytes per pixel (gray + alpha)
        .width = 128,
        .height = 128,
        .mipmaps = 1,
        .format = PIXELFORMAT_UNCOMPRESSED_GRAY_ALPHA
    };

    unsigned char id = 0;
    for (int y = 0; y < 16; y++){
        for (int x = 0; x < 16; x++){
            for(int l = 0; l < 8; l++){
                
                int pos = ((128*y+x)*8) + l * 128;
                for (int b = 7; b >= 0; b--){

                if (BIT_CHECK(fontData[id].bytes[l], b))
                    ((unsigned short *)imFont.data)[pos+7-b] = 0xffff;
                else 
                    ((unsigned short *)imFont.data)[pos+7-b] = 0x00ff;
                }
            }           
            id++;
        }
    }

    font.texture = LoadTextureFromImage(imFont);
    // Reconstruct charSet using charsWidth[], charsHeight, charsDivisor, glyphCount
    //------------------------------------------------------------------------------

    // Allocate space for our characters info data
    // NOTE: This memory must be freed at end! --> Done by CloseWindow()
    font.glyphs = (GlyphInfo *)RL_MALLOC(font.glyphCount*sizeof(GlyphInfo));
    font.recs = (Rectangle *)RL_MALLOC(font.glyphCount*sizeof(Rectangle));

    for (int i = 0; i < font.glyphCount; i++)
    {
        font.glyphs[i].value = i + 33;

        font.recs[i].x = (float)(i % 16 * charsWidth);
        font.recs[i].y = (float)(i / 16 * charsHeight);
        font.recs[i].width = (float)charsWidth;
        font.recs[i].height = (float)charsHeight;

        // NOTE: On default font character offsets and xAdvance are not required
        font.glyphs[i].offsetX = 0;
        font.glyphs[i].offsetY = 0;
        font.glyphs[i].advanceX = 0;
        // Fill character image data from fontClear data
        font.glyphs[i].image = ImageFromImage(imFont, font.recs[i]);
    }

    UnloadImage(imFont);

    font.baseSize = (int)font.recs[0].height;
SaveFont();
}
Font Tools::GetFont(){
    return font;
}
void Tools::SaveFont(){
    for (int i = 0; i<223; i++){
        printf("{");
        for (int b = 0; b<=7; b++){
            printf("%i%s",fontData[i].bytes[b],b == 7 ? "":",");
        }
        printf("}, // %i %c\n", i+33, (char)(i+33));
    }
}
void Tools::SetFontChar(unsigned int id, unsigned char b0, unsigned char b1, unsigned char b2, 
                                unsigned char b3, unsigned char b4, unsigned char b5, unsigned char b6,
                                unsigned char b7){
    id -= 33;
    if (id<0) id = 0;
    if (id>223) id = 223; 
    fontData[id].bytes[0] = b0;
    fontData[id].bytes[1] = b1;
    fontData[id].bytes[2] = b2;
    fontData[id].bytes[3] = b3;
    fontData[id].bytes[4] = b4;
    fontData[id].bytes[5] = b5;
    fontData[id].bytes[6] = b6;
    fontData[id].bytes[7] = b7;

    SetFont();
}
unsigned char Tools::GetFontByte(unsigned int id, unsigned char byte){
    id -= 33;
    if (id<0) id = 0;
    if (id>223) id = 223; 
    return fontData[id].bytes[byte];
}
int Tools::GetVirtualMouse(bool isXAxis){   
	float screenScale = Min((float)GetScreenWidth()/GAME_SCREEN_W,(float)GetScreenHeight()/GAME_SCREEN_H);
	float mouse = isXAxis ? GetMousePosition().x : GetMousePosition().y;
    float screen = isXAxis ? GetScreenWidth() : GetScreenHeight();
    float gamescreen = isXAxis ? GAME_SCREEN_W : GAME_SCREEN_H;
    float value = std::ceil((mouse - (screen - (gamescreen * screenScale)) * 0.5f) / screenScale);
    return (int)Clamp(value, 0,(float)gamescreen);
}
void Tools::SetVirtualMouse(int x,int y){   
	float screenScale = Min((float)GetScreenWidth()/GAME_SCREEN_W,(float)GetScreenHeight()/GAME_SCREEN_H);
    SetMousePosition(x * screenScale, y * screenScale);
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

std::string Tools::GetCharFromCodepoint(int codepoint) {
  
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
std::stringstream Tools::GetFiles(const char *path) {
    
    namespace fs = std::filesystem;
    const fs::path current_path = fs::current_path() / ASSETS_FOLDER / path;
    std::stringstream result;

    DIR *dir = opendir(current_path.string().c_str());
   
    if (dir == NULL) {
        return result;
    }

    for(auto& p : std::filesystem::directory_iterator(current_path)){
        if (p.is_regular_file()){
            std::string file = p.path().stem().string();
            std::string ext = p.path().extension().string();
            ext = ToUpper((char*)ext.c_str());
            if (ext == PROGRAM_EXTENSION)
                result  << file << "\n";
        }
    }
            
    closedir(dir);
    return result;
}
std::stringstream Tools::GetFolders(const char *path) {
    
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
            result << folder << "\n";
        }
    }
        
    closedir(dir);
    return result;
}
std::stringstream Tools::GetDir(const char *path) {
    
    std::stringstream folders = GetFolders(path);
    std::stringstream files = GetFiles(path);
    std::stringstream result;
    result << folders.str() << files.str();
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

float Tools::Min(float a, float b){
    return a<b ? a : b;
}