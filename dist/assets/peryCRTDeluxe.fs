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

const vec2 textureSize = vec2(320,200);

out vec4 finalColor;

//Vignette
vec3 vignette(vec2 uv, vec3 blur){
	uv *= 1.0 - uv.xy;
	if (uv.x < 0 || uv.y < 0) 
		return 0.2 * blur * (0.5/(uVignetteIntensity + 0.01));

	float vig = abs((16.0*uv.x*uv.y*(1.0-uv.x)*(1.0-uv.y)));
	return vec3(clamp(pow(vig, uVignetteIntensity),0.0 ,1.0));
}

vec3 gamma(vec3 color, float outputGamma){
   return pow(color, vec3(1.0 / outputGamma));
}

//Noise
float rand(vec2 co){
	float speed = 0.001 * uTime;
	return fract((sin( dot(co.xy , vec2(100.0 * speed, 1.0 * speed) )) * 16818.0 + speed));
}
 	
float noise(vec2 p) {
	p *= textureSize;
	vec2 ip = floor(p);
	vec2 u = fract(p);
	u = u*u*(3.0-2.0*u);
	float res = mix(
		mix(rand(ip),rand(ip+vec2(1.0,0.0)),u.x),
		mix(rand(ip+vec2(0.0,1.0)),rand(ip+vec2(1.0,1.0)),u.x),u.y);
	return res*res;
}
float gaus(float pos, float scale) {
	return exp2(scale*pos*pos);
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
	float f = 100.0;
	if (uv.y<0 && uv.x<0) return abs((uv.y + uv.x )*f);
	if (uv.x<0) r = abs(uv.x*f);
	if (uv.y<0) r = abs(uv.y*f);
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
    vec3 grille = texture2D(grilleTexture, uv * resolution.y * 0.125).rgb;

    float noiseF = mix(0.92, 1, noise(uv));
	float fliker = mix(0.98, 1, (sin(60.0*uTime+uv.y*4.0)*0.5+0.5));
	
    vec3 blur = gamma(blurColor * (5 * uBlurPower + 0.05), 5 * (uBlurFactor * uBlurFactor) + 0.05);
 	float scanlineF = (lines(uv.y, 200, uScanline));
 	float verticalLineF = (lines(uv.x, 320, uVerticalLine));
	
    vec3 blur2 = blur * blur * blur;
	vec3 scanline = mix(blur2 ,vec3(1.0), scanlineF);
	vec3 verticalLine = mix(blur2 ,vec3(1.0), verticalLineF);
	
	texelColor += blur2;
    texelColor *= scanline;
	texelColor *= verticalLine;

    texelColor *= noiseF;
    texelColor *= fliker;

    texelColor *= vignette(uv, blur);
	texelColor *= border(uv);

	//if (uv.x > 0.0) 
	texelColor *= gamma(grille,uGrilleForce*10);

    finalColor = vec4(texelColor, 1);
}