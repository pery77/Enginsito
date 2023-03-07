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
	if (uv.x < 0 || uv.y < 0) return blur * 0.05 * (1.0/(uVignetteIntensity + 0.5));

	float vig = abs((16.0*uv.x*uv.y*(1.0-uv.x)*(1.0-uv.y)));
	return vec3(clamp(pow(vig, uVignetteIntensity * 0.2),0.0 ,1.0));
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
	return exp2(scale*pos*pos);
}

// Return scanline weight.
float scan(vec2 pos, float off) {
	float dst = dist(pos).y;
	if (pos.x < 0 || pos.y < 0 || pos.x > 1 || pos.y > 1) return 1;
	return gaus(dst + off, (0.25+uScanline) * -8);
}

vec2 curve(vec2 uv){
	uv = (uv - 0.5) * 2.0;
	uv *= 1.1;	
	uv.x *= 1.0 + pow((abs(uv.y) / 5.0* uCurvature*0.25), 2.0 );
	uv.y *= 1.0 + pow((abs(uv.x) / 4.0* uCurvature*0.25), 2.0 );
	uv  = (uv / 2.0) + 0.5;
	uv =  uv *0.92 + 0.04;
	return uv;
}
float border(vec2 uv){
	uv *= 1.0 - uv.xy;
	float r = 1.0;
	float f = 100.0;
	if (uv.x<0) r = abs(uv.x*f);
	if (uv.y<0) r = abs(uv.y*f);
	if (uv.y<0 && uv.x<0) r = 1.0;
	return r;
}
void main(){

	vec2 uv = curve(fragTexCoord);
	//Chromatic
    float texelR = texture2D(texture0, vec2(uv.x + (0.3 * uChromatic * 0.003125), uv.y)).r;
    float texelG = texture2D(texture0, uv).g;
    float texelB = texture2D(texture0, vec2(uv.x - (0.3 * uChromatic * 0.003125), uv.y)).b;
	//Textures
    vec3 texelColor = vec3(texelR, texelG, texelB);
    vec3 blurColor = texture2D(blurTexture, uv).rgb;
    vec3 grille = texture2D(grilleTexture, uv * resolution.x * grilleLevel).rgb;

    float noiseF = mix(0.92, 1, noise(uv));
	float fliker = mix(0.98, 1, (sin(60.0*uTime+uv.y*4.0)*0.5+0.5));
	float scanline = scan(uv,-1.0) + scan(uv,0.0) + scan(uv,1.0);
    vec3 blur = gamma(blurColor * (uBlurPower + 0.05), uBlurFactor + 0.05);

    texelColor += blur * blur;
    texelColor *= mix(0.5,1.0,scanline);
    texelColor *= noiseF;
    texelColor *= fliker;
	//texelColor += grille * (grilleForce);
    texelColor *= vignette(uv, blurColor);
	texelColor *= border(uv);
	

    finalColor = vec4(texelColor, 1);

}