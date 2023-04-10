#shader vertex
#version 330 core

layout(location = 0) in vec2 aPos;
layout(location = 1) in vec2 aTexCoords;

uniform mat4 uModel;
uniform mat4 uViewProjection;
uniform vec2 uBaseScale;

out vec2 vTexCoords;

void main() {
    gl_Position = uViewProjection * uModel * vec4(aPos.x * uBaseScale.x, aPos.y * uBaseScale.y, 0.0, 1.0);
    vTexCoords = aTexCoords;
}

#shader fragment
#version 330 core

layout(location = 0) out vec4 gAlbedo;
layout(location = 1) out int gDrawID;

in vec2 vTexCoords;

uniform sampler2D uTexture;
uniform vec4 uTint;
uniform int uDrawID;
uniform vec4 uTexCrop; // (x1, y1, x2, y2)

float lerp(float from, float to, float t) {
    return from + ((to - from) * t);
}

void main() {
    vec2 texCoord = vTexCoords;
    texCoord.x = lerp(uTexCrop.x, uTexCrop.z, vTexCoords.x);
    texCoord.y = lerp(uTexCrop.y, uTexCrop.w, vTexCoords.y);
    gAlbedo = texture(uTexture, texCoord) * uTint;
    gDrawID = uDrawID;

    if(gAlbedo.a < 0.1)
        discard;
}