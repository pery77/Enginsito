#version 330

// Input vertex attributes (from vertex shader)
in vec2 fragTexCoord;
in vec4 fragColor;

// Input uniform values
uniform sampler2D texture0;
uniform sampler2D blurTexture;

uniform vec2 resolution;
uniform float uTime;
uniform float test;

uniform float uBlurPower;
uniform float uBlurFactor;

// Output fragment color
out vec4 finalColor;

// NOTE: Add here your custom variables

float vignetteIntensity = 0.25;

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


void main()
{
    vec2 uv = fragTexCoord;

    vec3 texelColor = texture2D(texture0, uv).rgb;
    vec3 blurColor = texture2D(blurTexture, uv).rgb;

    texelColor += gamma(blurColor * uBlurPower, uBlurFactor);


    float noiseF = mix(0.96, 1, noise(uv * 320));
	float fliker = mix(0.98, 1, (sin(60.0*uTime+uv.y*4.0)*0.5+0.5));

    texelColor *= vignette(uv) * noiseF * fliker;

    finalColor = vec4(texelColor, 1);
}