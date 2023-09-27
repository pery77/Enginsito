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
    
    void SetSprite(unsigned int id, uint8_t b0, uint8_t b1, uint8_t b2, 
                     uint8_t b3, uint8_t b4, uint8_t b5, uint8_t b6,
                     uint8_t b7);
    void DrawSprite(int id, int x, int y, int col, int flag);
    void DrawMetaSprite(int id, int x, int y, int metaFlag);
    uint8_t GetSpriteByte(unsigned int id, uint8_t byte);

    void AddMetaSprite(uint8_t id, uint8_t postition, uint8_t sprite_id, uint8_t offset_x, uint8_t offset_y, 
                         uint8_t color, uint8_t flags);
    std::array<int,20> GetMetaSprite(uint8_t id);

    void UpdateSpritesFromMemroy();

    private:

    void InitSprites();
    void InitFont();
    void SetPixels(uint8_t id);


};