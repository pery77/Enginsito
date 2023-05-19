#pragma once
#include "../../libs/raylib/include/raylib.h"
#include "tools.h"

#include "grille1.h"
#include "grille2.h"
#include "grille3.h"

struct BlurPass
{
	int passType; //Pass 0 downScale, other upScale
	float offset;
};

enum CRTProperty
{
    BlurPower,
    BlurFactor,
    Chromatic,
    Curvature,
    Vignetting,
    ScanLine,
    VerticalLine,
    GrilleForce,
    Noise,
    Fliker
};

class Engine;

class PostProcessing
{
    public:
    PostProcessing(Engine* _engine);
    ~PostProcessing();

    Vector2i resolution16_10[6] 
    {
        {3840,2400}, 
        {2560,1600},
        {1920,1200},
        {1680,1050},
        {1440,900},
        {1280,800}
    };

    void RenderMain();
    void RenderBlur();
    void RenderFinal();

    bool enabled = true;

    RenderTexture2D mainRender;
    RenderTexture2D editorRender;

    float uTime;
    float uTest;

    float uBlurPower = 0.19;
    float uBlurFactor = 0.6;
    float uChromatic = 0.31;
    float uCurvature = 0.39;
    float uVignetteIntensity = 0.19;
    float uScanline = 0.19;
    float uVerticalLine = 0.03;
    float uGrilleForce = 0.5;
    float uNoise = 0.19;
    float uFliker = 0.03;

    void ReloadShaders();

    void UpdateGameScreenRects();
    void UpdateWindowSize();
    void FullScreen();

    void SetState(bool newState);

	float screenScale;
	float previusWindowsWidth;
	float previusWindowsHeight;
	float currentAspectRatio;
    float editorImageFactor;

    void SetCRTValue(CRTProperty property, uint8_t value);
    void SetGrilleTexture(int newTextureId);

    private:

    Rectangle gameRect  { 0, 0, (float)(GAME_SCREEN_W), -(float)(GAME_SCREEN_H)};
    Rectangle gameScaledRect { 0, 0, (float)GetScreenWidth(), (float)GetScreenHeight()};

    void setUpShaders();
    float GetCRTFloat(CRTProperty property);

    RenderTexture2D bufferTexture;

    Texture grilleTextures[3];
    int currentGrilleTexture = 0;
    Shader crtShader;
    Shader blurShader;

	BlurPass blurPasses[5] {{0,2},{0,3},{0,8},{1,2},{1,3}};

    // Blur shader locations
    int pass;
    int resolutionLoc;
    int offsetLoc;

    // CRT shader locations
    // Internal
    int grilleLoc;
    int blurTextureLoc;
    int resolutionCRTLoc;
    int timeLoc;

    // User
    int blurPowerLoc;
    int blurFactorLoc;
    int chromaticLoc;
    int curvatureLoc;
    int vignetteIntensityLoc;
    int scanlineLoc;
    int verticalLineLoc;
    int grilleForceLoc;
    int noiseLoc;
    int flikerLoc;


    Vector2 resolution {(float)GetScreenWidth(), (float)GetScreenHeight()};
    Texture textureFromCode(int format, int height, int width, void* data);

    const char* blurShaderCode = R"|(
#version 330

// Input vertex attributes (from vertex shader)
in vec2 fragTexCoord;
in vec4 fragColor;

// Input uniform values
uniform sampler2D texture0;
uniform int pass;
uniform vec2 resolution;
uniform float offset;

// Output fragment color
out vec4 finalColor;

void main()
{
    vec2 uv = fragTexCoord.xy;
    vec2 halfpixel = 0.5 / (resolution.xy / 2.0);
    vec4 texel;

    if (pass == 0){
        //Down
        texel = texture2D(texture0, uv) * 4.0;
        texel += texture2D(texture0, uv - halfpixel.xy * offset);
        texel += texture2D(texture0, uv + halfpixel.xy * offset);
        texel += texture2D(texture0, uv + vec2(halfpixel.x, -halfpixel.y) * offset);
        texel += texture2D(texture0, uv - vec2(halfpixel.x, -halfpixel.y) * offset);
        texel /= 8;
    }else{
        //Up
        texel = texture2D(texture0, uv + vec2(-halfpixel.x * 2.0, 0.0) * offset);
        texel += texture2D(texture0, uv + vec2(-halfpixel.x, halfpixel.y) * offset) * 2.0;
        texel += texture2D(texture0, uv + vec2(0.0, halfpixel.y * 2.0) * offset);
        texel += texture2D(texture0, uv + vec2(halfpixel.x, halfpixel.y) * offset) * 2.0;
        texel += texture2D(texture0, uv + vec2(halfpixel.x * 2.0, 0.0) * offset);
        texel += texture2D(texture0, uv + vec2(halfpixel.x, -halfpixel.y) * offset) * 2.0;
        texel += texture2D(texture0, uv + vec2(0.0, -halfpixel.y * 2.0) * offset);
        texel += texture2D(texture0, uv + vec2(-halfpixel.x, -halfpixel.y) * offset) * 2.0;
        texel /= 12.0;
    }

    finalColor = clamp(texel, 0.0, 1.0);
}
)|";
const char* crtShaderCode = R"|(
#version 330

in vec2 fragTexCoord;
in vec4 fragColor;

uniform sampler2D texture0;
uniform sampler2D blurTexture;
uniform sampler2D grilleTexture;

uniform vec2 resolution;
uniform float uTime;

uniform float uBlurPower  = 0.85;
uniform float uBlurFactor = 1.2;

uniform float uChromatic = 0.3;
uniform float uCurvature = 0.15;
uniform float uVignetteIntensity = 0.2;
uniform float uScanline = 0.5;

uniform float uVerticalLine = 0.08;
uniform float uGrilleForce = 0.2;

uniform float uNoise = 0.8;
uniform float uFliker = 0.94;



const vec2 textureSize = vec2(320,200);

out vec4 finalColor;

//Vignette
vec3 vignette(vec2 uv, vec3 blur){
	uv *= 1.0 - uv.xy;

	if (uv.x < 0 || uv.y < 0){
		return clamp(0.2 * blur * (0.75/(uVignetteIntensity + 0.01)),0 ,1);
	}

	float vUvs = uv.x*uv.y*(1.0-uv.x)*(1.0-uv.y);
	float vigF = abs(2000.0*vUvs);
	float vig = abs(16.0*vUvs);

	vec3 vigIn= vec3(clamp(pow(vigF, 0.5) ,0.0 ,1.0));
	return vec3(clamp(pow(vig, uVignetteIntensity),0.0 ,1.0)) * vigIn;
}

vec3 gamma(vec3 color, float outputGamma){
   return pow(color, vec3(1.0 / outputGamma));
}

//Noise
float rand(vec2 co){
	float speed = 0.0001 * uTime;
	return fract((sin( dot(co.xy , vec2(speed, 100*speed) )) * 16818.0 + speed));
}
 	
float noise(vec2 p) {
	p *= textureSize * 0.9;
	vec2 ip = floor(p);
	vec2 u = fract(p);
	u = u*u*(3.0-2.0*u);
	float res = mix(
		mix(rand(ip),rand(ip+vec2(1.0,0.0)),u.x),
		mix(rand(ip+vec2(0.0,1.0)),rand(ip+vec2(1.0,1.0)),u.x),u.y);
	return res*res;
}

vec2 curve(vec2 uv){
	if (uCurvature < 0.001) return uv;
	uv = (uv - 0.5) * 2.0;
	uv *= 1.1;	
	uv.x *= 1.0 + pow((abs(uv.y) / 5.0 * uCurvature * 2), 2.0 );
	uv.y *= 1.0 + pow((abs(uv.x) / 4.0 * uCurvature * 2), 2.0 );
	uv  = (uv / 2.0) + 0.5;
	uv =  uv *0.92 + 0.04;
	return uv;
}

float border(vec2 uv){
	uv *= 1.0 - uv.xy;
	float r = 1.0;
	float f = 60.0;
	if (uv.y<0 && uv.x<0) return clamp(abs((uv.y + uv.x )*f),0,1);
	if (uv.x<0) r = clamp(abs(uv.x*f),0,1);
	if (uv.y<0) r = clamp(abs(uv.y*f),0,1);
	return r;
}

float lines(float x, float n, float e){
	return clamp(abs(sin(x * 3.141592 * n) * e) + 1 - e, 0, 1);
}

vec3 mainImage(vec2 uv){
	//Chromatic
    float texelR = texture2D(texture0, vec2(uv.x + (uChromatic * 0.003125), uv.y)).r;
    float texelG = texture2D(texture0, uv).g;
    float texelB = texture2D(texture0, vec2(uv.x - (uChromatic * 0.003125), uv.y)).b;
	
	uv *= 1.0 - uv.xy;
	if (uv.x < 0 || uv.y < 0) 
		return vec3(0);
	
    return vec3(texelR, texelG, texelB);
}

void main(){
	//Uv
	vec2 uv = curve(fragTexCoord);

	//Textures
	vec3 texelColor = mainImage(uv);
    vec3 blurColor = texture2D(blurTexture, uv).rgb;
    vec3 grille = texture2D(grilleTexture, uv * resolution.y * 0.16).rgb;

    float noiseF = mix((1-uNoise)*0.5 + 0.5, 1, noise(uv));
	float fliker = mix((1-uFliker)*0.5 + 0.5, 1, (sin((30 * uTime) + (uv.y * 6.28))*0.5+0.5));
	
    vec3 blur = gamma(blurColor * (5 * uBlurPower + 0.05), 5 * (uBlurFactor * uBlurFactor) + 0.05);
 	float scanlineF = (lines(uv.y, textureSize.y, uScanline));
 	float verticalLineF = (lines(uv.x, textureSize.x, uVerticalLine));
    
    vec3 blur2 = blur * blur * blur;
	blur2 = clamp(blur2, 0.0, 100.0);
	
    vec3 scanline = mix(blur2 ,vec3(1.0), scanlineF);
	vec3 verticalLine = mix(blur2 ,vec3(1.0), verticalLineF);
	
	texelColor += blur2;
    texelColor *= scanline;
	texelColor *= verticalLine;

    texelColor *= noiseF;
    texelColor *= fliker;

    texelColor *= vignette(uv, blur);
	texelColor *= border(uv);

	//if (uv.x > 0.5) 
	texelColor *= mix(vec3(1.0), gamma(grille,2.2), uGrilleForce);

    finalColor = vec4(texelColor, 1);
}
)|";
};