#version 300 es
precision mediump float;

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

uniform mat4 uModel;
uniform mat4 uViewProjection;

uniform mat4 uView;
uniform mat4 uProj;


out vec2 vTexCoords;
out vec3 vNormal;
out vec3 vModelPos;

void main()
{
    vModelPos = vec3(uModel * vec4(aPos, 1.0f));

    gl_Position = uProj * uView * uModel * vec4(aPos.x, aPos.y, aPos.z, 1.0);
    

    vTexCoords = aTexCoords;
    vNormal = aNormal;
}