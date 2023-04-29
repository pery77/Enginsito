#pragma once
#include "tools.h"

class Engine;

class SpriteManager
{
    public:

    SpriteManager(Engine* _engine);
    ~SpriteManager();

    Font font = {0};
    float fontSpacing = 0;
    Texture spriteTexture;
    Image imgSprite;

    void SetFontSpacing(int spacing);

    Color GetColor(int color);
    Color GetBiosColor(int color);
    Color SetColor(int color, int r, int g, int b);
    
    void SetSprite(unsigned int id, unsigned char b0, unsigned char b1, unsigned char b2, 
                     unsigned char b3, unsigned char b4, unsigned char b5, unsigned char b6,
                     unsigned char b7);
    void DrawSprite(int id, int x, int y, int col, int flag);
    void DrawMetaSprite(int id, int x, int y);
    unsigned char GetSpriteByte(unsigned int id, unsigned char byte);

    void AddMetaSprite(unsigned char id, unsigned char postition, unsigned char sprite_id, unsigned char offset_x, unsigned char offset_y, 
                         unsigned char color, unsigned char flags);
    std::array<int,20> GetMetaSprite(unsigned char id);

    void SetPixels(uint8_t id);

    private:

    void InitSprites();
    void InitFont();


};