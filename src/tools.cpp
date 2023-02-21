#include "tools.h"
unsigned char MainMemory[4096]{};

MetaSprites metaSprites[255] {}; 

Font font = {0};

Color userPalette[16]{};

Texture spriteTexture;
Image imgSprite;

float fontSpacing = 0;

void Tools::SetFontSpacing(int spacing){
    fontSpacing = spacing;
}

float Tools::GetFontSpacing(){
    return fontSpacing;
}

int Tools::IntClamp(int value, int min, int max){
    if (value < min) return min;
    if (value > max) return max;
}

Color Tools::GetColor(int color){
    color = IntClamp(color, 0, 15);
    return (Color){Peek(color * 3), Peek(color * 3 + 1), Peek(color * 3 + 2), 255};
}

Color Tools::GetBiosColor(int color){
    return GetColor(color);
}

Color Tools::SetColor(int color, int r, int g, int b){
    Poke(color * 3, r);
    Poke(color * 3 + 1, g);
    Poke(color * 3 + 2, b);
}

void Tools::UpdateFont(){

    UnloadFont(font);
    RenderSprites();

    font.glyphCount = 255;   // Number of chars included in our default font
    font.glyphPadding = 0;   // Characters padding
    int charsHeight = 8;
    int charsWidth = 8;

    font.texture = spriteTexture;
    font.glyphs = (GlyphInfo *)RL_MALLOC(font.glyphCount*sizeof(GlyphInfo));
    font.recs = (Rectangle *)RL_MALLOC(font.glyphCount*sizeof(Rectangle));

    for (int i = 0; i < font.glyphCount; i++)
    {
        font.glyphs[i].value = i;

        font.recs[i].x = (float)(i % 16 * charsWidth);
        font.recs[i].y = (float)(i / 16 * charsHeight);
        font.recs[i].width = (float)charsWidth;
        font.recs[i].height = (float)charsHeight;

        font.glyphs[i].offsetX = 0;
        font.glyphs[i].offsetY = 0;
        font.glyphs[i].advanceX = 0;
        font.glyphs[i].image = ImageFromImage(imgSprite, font.recs[i]);
    }

    font.baseSize = (int)font.recs[0].height;
}
Font Tools::GetFont(){
    return font;
}

void Tools::RenderSprites(){

    #define BIT_CHECK(a,b) ((a) & (1 << (b)))
    if (imgSprite.data == 0x0){
        imgSprite = {
            .data = calloc(128*128, 2),  // 2 bytes per pixel (gray + alpha)
            .width = 128,
            .height = 128,
            .mipmaps = 1,
            .format = PIXELFORMAT_UNCOMPRESSED_GRAY_ALPHA
        };
    }
    //TODO: 1D FOR LOOP
    unsigned char id = 0;
    for (int y = 0; y < 16; y++){
        for (int x = 0; x < 16; x++){
            for(int l = 0; l < 8; l++){
                
                int pos = ((128*y+x)*8) + l * 128;
                for (int b = 7; b >= 0; b--){

                if (BIT_CHECK(Peek((id * 8) + 48 + l), b))
                    ((unsigned short *)imgSprite.data)[pos+7-b] = 0xffff;
                else 
                    ((unsigned short *)imgSprite.data)[pos+7-b] = 0x00ff;
                }
            }           
            id++;
        }
    }
    UnloadTexture(spriteTexture);
    spriteTexture = LoadTextureFromImage(imgSprite);
}

void Tools::SetSprite(unsigned int id, unsigned char b0, unsigned char b1, unsigned char b2, 
                                unsigned char b3, unsigned char b4, unsigned char b5, unsigned char b6,
                                unsigned char b7){
    id = IntClamp(id, 0, 255);
    id = id * 8 + 48;

    Poke(id,     b0);
    Poke(id + 1, b1);
    Poke(id + 2, b2);
    Poke(id + 3, b3);
    Poke(id + 4, b4);
    Poke(id + 5, b5);
    Poke(id + 6, b6);
    Poke(id + 7, b7);

}

Texture Tools::GetSpriteTexture(){
    return spriteTexture;
}

unsigned char Tools::GetSpriteByte(unsigned int id, unsigned char byte){
    id = IntClamp(id, 0, 255);
    return Peek((id * 8) + 48 + byte);
}

void Tools::AddMetaSprite(unsigned char id,unsigned char postition, unsigned char sprite_id, unsigned char offset_x, unsigned char offset_y, 
                                            unsigned char color, unsigned char flags){
    metaSprites[id].sprites[postition] = (SpriteData){sprite_id, offset_x, offset_y, color, flags};
}

void Tools::ClearMetaSprite(unsigned char id){
    for ( unsigned char i=0; i<8 ; i++){
        metaSprites[id].sprites[i] = (SpriteData){0, 0, 0, 0, 255};
    }   
}

std::array<int,40> Tools::GetMetaSprite(unsigned char id){
    std::array<int,40> r;
    int c = 0;
    for (unsigned char i=0; i<8 ; i++){
        for (unsigned char b=0; b<5 ; b++){
            r[c++] = (int) metaSprites[id].sprites[i].bytes[b];
        }
    }

    return r;
}


void Tools::DrawSprite(int id, int x, int y, int col, int flag){
    if (flag == 255) return;
    float rot = 0;
	Vector2 pivot {0,0};

	switch ((flag & 0b11))
	{
		case 1:
			rot = 90;
			pivot = (Vector2){0,8};
			break;
		case 2:
			rot = 180;
			pivot = (Vector2){8,8};
			break;
		case 3:
			rot = -90;
			pivot = (Vector2){8,0};
			break;
	default:
		break;
	}

	int h = 8;
	int v = 8;
	if(flag & (1 << 3)) h = -8;
	if(flag & (1 << 4)) v = -8;

    DrawTexturePro(GetSpriteTexture(), (Rectangle){(id%16)*8,(id/16)*8,h,v}, 
	(Rectangle){x,y,8,8}, pivot, rot, GetColor(col));
}

void Tools::DrawMetaSprite(int id, int x, int y){
    for ( unsigned char i=0; i<7 ; i++){
        SpriteData mp = metaSprites[id].sprites[i];
    
        if (mp.bytes[4] == 255) break;
        DrawSprite(mp.bytes[0], mp.bytes[1] + x, mp.bytes[2] + y, mp.bytes[3], mp.bytes[4]);
    }
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
            if (IsFileExtension(p.path().filename().string().c_str(), PROGRAM_EXTENSION))
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

void Tools::DumpMemory(const char *path){
//for (int i = 0; i < 256;i++){
//    for ( int j = 0; j<8;j++){
//
//        MainMemory[(i * 8) + 48 + j] = memoryData[i].bytes[j];
//    }
//}

    FILE *f = fopen(path, "wb");
    if (f) {
        size_t r = fwrite(MainMemory, sizeof(MainMemory[0]), 4096, f);
        printf("Wrote %zu elements\n", r);
        fclose(f);
    }
}

void Tools::LoadMemory(const char *path){
    FILE *f = fopen(path, "rb");
    if (f) {
        size_t r = fread(MainMemory, sizeof(MainMemory), 1, f);
        printf("Readed %zu elements\n", r);
        fclose(f);
    }
}

unsigned char Tools::Peek(unsigned int dir){
    return (MainMemory)[dir];
}

void Tools::Poke(unsigned int dir, unsigned char value){
    value = IntClamp(value, 0, 255);
    MainMemory[dir] = value;
}