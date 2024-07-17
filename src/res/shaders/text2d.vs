#version 300 es
precision mediump float;

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aUV;
layout(location = 2) in vec4 aColor;
layout(location = 3) in uint aDrawID;
layout(location = 4) in float aTexture;

uniform mat4 uProj;
uniform mat4 uView;

out vec4 vColor;
out vec2 vUV;
flat out uint vDrawID;
out float vTexture;

void main() {
  gl_Position = uProj * uView * vec4(aPos.x, aPos.y, aPos.z, 1.0);

  vColor = aColor;
  vUV = aUV;
  vDrawID = aDrawID;
  vTexture = aTexture;
}
