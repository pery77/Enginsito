#version 330

// Input vertex attributes
in vec3 vertexPosition;
in vec2 vertexTexCoord;

// Input uniform values
uniform mat4 mvp;

// Output vertex attributes (to fragment shader)
out vec2 fragTexCoord;

// NOTE: Add here your custom variables


void main(){

    fragTexCoord = vertexTexCoord;

    // Calculate final vertex position
    gl_Position = mvp*vec4(vertexPosition, 1.0);
}