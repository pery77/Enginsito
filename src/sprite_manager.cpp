#include "sprite_manager.h"
#include "engine.h"

Font font = {0};
float fontSpacing = 0;
Texture spriteTexture;
Image imgSprite;

Engine* spriteEngineRef;

SpriteManager::SpriteManager(Engine* _engine)
{
    spriteEngineRef = _engine;
    imgSprite = {
            .data = calloc(128*128, 2),  // 2 bytes per pixel (gray + alpha)
            .width = 128,
            .height = 128,
            .mipmaps = 1,
            .format = PIXELFORMAT_UNCOMPRESSED_GRAY_ALPHA
        };

    InitSprites();
    InitFont();

}

SpriteManager::~SpriteManager()
{

}

void SpriteManager::SetFontSpacing(int spacing)
{
    fontSpacing = spacing;
}

void SpriteManager::InitFont()
{
    font.glyphCount = 255;   // Number of chars included in our default font
    font.glyphPadding = 0;   // Characters padding
    int charsHeight = 8;
    int charsWidth = 8;

    font.texture = spriteTexture;
    font.glyphs = (GlyphInfo *)RL_MALLOC(font.glyphCount*sizeof(GlyphInfo));
    font.recs = (Rectangle *)RL_MALLOC(font.glyphCount*sizeof(Rectangle));

    for (int i = 0; i < font.glyphCount; i++) {
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

Color SpriteManager::GetColor(int color)
{
    color = Tools::IntClamp(color, 0, 15);
    return (Color){spriteEngineRef->Peek(color * 3), spriteEngineRef->Peek(color * 3 + 1), spriteEngineRef->Peek(color * 3 + 2), 255};
}

Color SpriteManager::GetBiosColor(int color)
{
    return GetColor(color);
}

Color SpriteManager::SetColor(int color, int r, int g, int b)
{
    spriteEngineRef->Poke(color * 3, r);
    spriteEngineRef->Poke(color * 3 + 1, g);
    spriteEngineRef->Poke(color * 3 + 2, b);
}

void SpriteManager::SetSprite(unsigned int id, uint8_t b0, uint8_t b1, uint8_t b2, 
                                uint8_t b3, uint8_t b4, uint8_t b5, uint8_t b6,
                                uint8_t b7)
{
    id = Tools::IntClamp(id, 0, 255);
    unsigned int dir = (id * 8) + 48;

    spriteEngineRef->Poke(dir,     b0);
    spriteEngineRef->Poke(dir + 1, b1);
    spriteEngineRef->Poke(dir + 2, b2);
    spriteEngineRef->Poke(dir + 3, b3);
    spriteEngineRef->Poke(dir + 4, b4);
    spriteEngineRef->Poke(dir + 5, b5);
    spriteEngineRef->Poke(dir + 6, b6);
    spriteEngineRef->Poke(dir + 7, b7);

}

uint8_t SpriteManager::GetSpriteByte(unsigned int id, uint8_t byte) 
{
    id = Tools::IntClamp(id, 0, 255);
    return spriteEngineRef->Peek((id * 8) + 48 + byte);
}

void SpriteManager::AddMetaSprite(uint8_t id, uint8_t postition, uint8_t sprite_id, uint8_t offset_x, uint8_t offset_y, 
                                            uint8_t color, uint8_t flags)
{
    id = Tools::IntClamp(id, 0, 63);
    postition = Tools::IntClamp(postition, 0, 3);

    unsigned int dir = 2096 + ((id * 20) + (postition * 5));
    spriteEngineRef->Poke(dir , sprite_id);
    spriteEngineRef->Poke(dir + 1, offset_x);
    spriteEngineRef->Poke(dir + 2, offset_y);
    spriteEngineRef->Poke(dir + 3, color);
    spriteEngineRef->Poke(dir + 4, flags);
}

std::array<int,20> SpriteManager::GetMetaSprite(uint8_t id) 
{
    id = Tools::IntClamp(id, 0, 63);
    unsigned int dir = 2096 + (id * 20);
    std::array<int,20> r;
    int c = 0;
    for (uint8_t i=0; i<20; i++) {
        r[c++] = (int) spriteEngineRef->Peek(dir + i);
    }
    return r;
}

void SpriteManager::DrawMetaSprite(int id, int x, int y, int metaFlag = 0) 
{
    id = Tools::IntClamp(id, 0, 63);
    unsigned int dir = (id * 20) + 2096;
    for ( uint8_t i=0; i<=3; i++) 
    {
        if ((spriteEngineRef->Peek(dir + 4) >> 7) & 1) continue;

        int spId   = spriteEngineRef->Peek(dir + i * 5);
        int spX    = spriteEngineRef->Peek(dir + 1 + i * 5);
        int spY    = spriteEngineRef->Peek(dir + 2 + i * 5);
        int spCol  = spriteEngineRef->Peek(dir + 3 + i * 5);
        int spFlag = spriteEngineRef->Peek(dir + 4 + i * 5);
        
        int rotatedX, rotatedY;
	    switch ((metaFlag & 0b11))
	    {
            case 1: //90
                rotatedX = spY;
                rotatedY = spX;
                break;
            case 2: //180
                rotatedX = -spX;
                rotatedY = -spY;
                break;
            case 3: //270
                rotatedX = -spY;
                rotatedY = -spX;
                break;
            default: //0
                rotatedX = spX;
                rotatedY = spY;
            break;
	    }

        DrawSprite(spId, rotatedX + x, rotatedY + y, spCol, spFlag + metaFlag);
    }
}

void SpriteManager::InitSprites()
{
    #define BIT_CHECK(a,b) ((a) & (1 << (b)))

    uint8_t id = 0;
    for (int y = 0; y < 16; y++) {
        for (int x = 0; x < 16; x++) {
            for(int l = 0; l < 8; l++) {
                int pos = ((128*y+x)*8) + l * 128;
                for (int b = 7; b >= 0; b--) {
                    if (BIT_CHECK(spriteEngineRef->Peek((id * 8) + 48 + l), b))
                        ((unsigned short *)imgSprite.data)[pos+7-b] = 0xffff;
                    else 
                        ((unsigned short *)imgSprite.data)[pos+7-b] = 0x00ff;
                }
            }           
            id++;
        }
    }

    spriteTexture = LoadTextureFromImage(imgSprite);
}

void SpriteManager::DrawSprite(int id, int x, int y, int col, int flag) 
{
    if ((flag >> 7) & 1) return;
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

    DrawTexturePro(spriteTexture, (Rectangle){(id%16)*8,(id/16)*8,h,v}, 
	(Rectangle){x,y,8,8}, pivot, rot, GetColor(col));
}

void SpriteManager::SetPixels(uint8_t id)
{
    unsigned short  pixels[64] = { };
    int pixelIndex = 0;
    for (int l = 0; l < 8; l++)
    {
        uint8_t byte = spriteEngineRef->Peek((id * 8) + l + 48);
        for (int b = 7; b >= 0; b--)
        {
            pixels[pixelIndex++] = (byte & (1 << b)) ? 0xFFFF : 0x00FF;
        }
    }
 
    UpdateTextureRec(spriteTexture, (Rectangle){(id%16)*8,(id/16)*8, 8, 8}, &pixels);
}

void SpriteManager::UpdateSpritesFromMemroy()
{
    for (int i = 0; i<256; i++)
        SetPixels(i);
}