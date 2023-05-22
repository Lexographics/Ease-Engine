#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec4 aColor;
layout(location = 2) in vec2 aTexCoords;
layout(location = 3) in float aTexture;
layout(location = 4) in float aDrawId;
layout(location = 5) in float aDrawMode; // 0: normal, 1: text

uniform mat4 uProj;
uniform mat4 uView;

out vec4 vColor;
out vec2 vTexCoords;
out float vTexture;
out float vDrawId;
out float vDrawMode;

void main() {
    gl_Position = uProj * uView * vec4(aPos.x, aPos.y, aPos.z, 1.0);

    vColor = aColor;
    vTexCoords = aTexCoords;
    vTexture = aTexture;
    vDrawId = aDrawId;
    vDrawMode = aDrawMode;
}