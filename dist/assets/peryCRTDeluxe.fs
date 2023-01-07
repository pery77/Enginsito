#version 330

// Input vertex attributes (from vertex shader)
in vec2 fragTexCoord;
in vec4 fragColor;

// Input uniform values
uniform sampler2D texture0;
uniform sampler2D blurTexture;
uniform sampler2D grilleTexture;

uniform vec2 resolution;
uniform float uTime;
uniform float test;

uniform float uBlurPower;
uniform float uBlurFactor;

uniform float vignetteIntensity = 0.15;

uniform float hardScan = -8.0;
uniform float chromatic = 0.5;

const vec2 textureSize = vec2(320,200);

out vec4 finalColor;

//
float vignette(vec2 uv){
	uv *= 1.0 - uv.xy;
	float vignette = uv.x * uv.y * 15.0;
	return clamp(pow(vignette, vignetteIntensity),0.0, 1.0);
}

vec3 gamma(vec3 color, float outputGamma){
   return pow(color, vec3(1.0 / outputGamma));
}

//Noise
float rand(vec2 co){
	float speed = 0.001 * uTime;
	return fract((sin( dot(co.xy , vec2(100.0 * speed, 1.0 * speed) )) * 10000.0+ speed));
}
            
float noise (in vec2 st){
	vec2 i = floor(st);
	vec2 f = fract(st);

	// Four corners in 2D of a tile
	float a = rand(i);
	float b = rand(i + vec2(1.0, 0.0));
	float c = rand(i + vec2(0.0, 1.0));
	float d = rand(i + vec2(1.0, 1.0));

	// Smooth Interpolation
	// Cubic Hermine Curve.  Same as SmoothStep()
	vec2 u = f*f*(3.0-2.0*f);
	u = smoothstep(0.,1.,f);
	// Mix 4 coorners percentages
	return mix(a, b, u.x) +
       (c - a)* u.y * (1.0 - u.x) +
       (d - b) * u.x * u.y;
}


vec2 dist(vec2 pos) {
	pos = pos*vec4(textureSize, 1.0 / textureSize).xy;
	return -((pos - floor(pos)) - vec2(0.5));
}

float gaus(float pos, float scale) {
	return exp2(scale*pow(abs(pos), 2.0));
}

// Return scanline weight.
float scan(vec2 pos, float off) {
	float dst = dist(pos).y;
	return gaus(dst + off, hardScan);
}

void main()
{
    vec2 uv = fragTexCoord;

    float texelR = texture2D(texture0, vec2(uv.x + (chromatic * 0.003125), uv.y)).r;
    float texelG = texture2D(texture0, uv).g;
    float texelB = texture2D(texture0, vec2(uv.x - (chromatic * 0.003125), uv.y)).b;

    vec3 texelColor = vec3(texelR, texelG, texelB);
    vec3 blurColor = texture2D(blurTexture, uv).rgb;
    vec3 grille = texture2D(grilleTexture, uv * resolution.x * 0.1 ).rgb;

	texelColor += grille * 0.3;
    texelColor += gamma(blurColor * uBlurPower, uBlurFactor);


    float noiseF = mix(0.96, 1, noise(uv * 320));
	float fliker = mix(0.98, 1, (sin(60.0*uTime+uv.y*4.0)*0.5+0.5));
	float scanline = scan(uv,-1.0) + scan(uv,0.0) + scan(uv,1.0);

    texelColor *= vignette(uv) * noiseF * fliker * scanline;

    finalColor = vec4(texelColor, 1);
}