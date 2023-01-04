#version 330

// Input vertex attributes (from vertex shader)
in vec2 fragTexCoord;
in vec4 fragColor;

// Input uniform values
uniform sampler2D texture0;
uniform sampler2D blurTexture;

// Output fragment color
out vec4 finalColor;

// NOTE: Add here your custom variables

void main()
{
    // Texel color fetching from texture sampler
    vec4 texelColor = texture2D(texture0, fragTexCoord);
    vec4 blurColor = texture2D(blurTexture, fragTexCoord);
    texelColor *= blurColor;

    // Calculate final fragment color
    finalColor = vec4(texelColor.rgb, 1);
}