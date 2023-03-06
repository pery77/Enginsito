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

uniform float grilleForce = 0.2;
uniform float grilleLevel = 0.08;

const vec2 textureSize = vec2(320,200);

out vec4 finalColor;

//Vignette
vec3 vignette(vec2 uv, vec3 blur){
	uv *= 1.0 - uv.xy;

	if (uv.x < 0 || uv.y < 0) return blur * 0.15 * (1/(uVignetteIntensity + 0.05));
	float v = uv.x * uv.y * 15.0;
	return vec3(clamp(pow(v, uVignetteIntensity * 0.2),0.0, 1.0));
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
	return gaus(dst + off, uScanline * -8);
}

void main(){

    vec2 uv = fragTexCoord;

    //Curvature    
	vec2 dist  = .5 - uv;
    uv.x = (uv.x - dist.y * dist.y * dist.x * (uCurvature * 0.15)/(resolution.x/resolution.y));
    uv.y = (uv.y - dist.x * dist.x * dist.y * (uCurvature * 0.15));
	//Chromatic
    float texelR = texture2D(texture0, vec2(uv.x + (0.3 * uChromatic * 0.003125), uv.y)).r;
    float texelG = texture2D(texture0, uv).g;
    float texelB = texture2D(texture0, vec2(uv.x - (0.3 * uChromatic * 0.003125), uv.y)).b;
	//Textures
    vec3 texelColor = vec3(texelR, texelG, texelB);
    vec3 blurColor = texture2D(blurTexture, uv).rgb;
    vec3 grille = texture2D(grilleTexture, uv * resolution.x * grilleLevel ).rgb;

    float noiseF = mix(0.92, 1, noise(uv));
	float fliker = mix(0.98, 1, (sin(60.0*uTime+uv.y*4.0)*0.5+0.5));
	float scanline = scan(uv,-1.0) + scan(uv,0.0) + scan(uv,1.0);
    vec3 blur = gamma(blurColor * (uBlurPower + 0.05), uBlurFactor + 0.05);

    texelColor += blur;
    texelColor *= scanline;
    texelColor *= noiseF;
    texelColor *= fliker;
	texelColor += grille * (grilleForce);
    texelColor *= vignette(uv, blurColor);

    finalColor = vec4(texelColor, 1);

}