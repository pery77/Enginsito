#version 330

// Input vertex attributes (from vertex shader)
in vec2 fragTexCoord;
in vec4 fragColor;

// Input uniform values
uniform sampler2D texture0;
uniform sampler2D blurTexture;

uniform vec2 resolution;
uniform float test;

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

void main()
{
    vec2 uv = fragTexCoord;

    vec3 texelColor = texture2D(texture0, uv).rgb;
    vec3 blurColor = texture2D(blurTexture, uv).rgb;

    texelColor *= gamma(blurColor, 4.0);

    texelColor *= vignette(uv);

    finalColor = vec4(texelColor, 1);
}