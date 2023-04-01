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

    finalColor = texel;
}