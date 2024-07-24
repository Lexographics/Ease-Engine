#version 300 es
precision mediump float;

layout (location = 0) out vec4 color;
layout (location = 1) out uint drawId;

in vec4 vColor;
in vec2 vUV;
in float vTexture;
flat in uint vDrawID;

uniform sampler2D uTextures[16];


vec4 getTexture();

void main() {
  color = getTexture() * vColor;
  drawId = vDrawID;

  if(color.a < 0.1f)
    discard;
}


vec4 getTexture() {
  int index = int(vTexture);

  if(index == 0)
    return texture(uTextures[0], vUV);
  else if(index == 1)
    return texture(uTextures[1], vUV);
  else if(index == 2)
    return texture(uTextures[2], vUV);
  else if(index == 3)
    return texture(uTextures[3], vUV);
  else if(index == 4)
    return texture(uTextures[4], vUV);
  else if(index == 5)
    return texture(uTextures[5], vUV);
  else if(index == 6)
    return texture(uTextures[6], vUV);
  else if(index == 7)
    return texture(uTextures[7], vUV);
  else if(index == 8)
    return texture(uTextures[8], vUV);
  else if(index == 9)
    return texture(uTextures[9], vUV);
  else if(index == 10)
    return texture(uTextures[10], vUV);
  else if(index == 11)
    return texture(uTextures[11], vUV);
  else if(index == 12)
    return texture(uTextures[12], vUV);
  else if(index == 13)
    return texture(uTextures[13], vUV);
  else if(index == 14)
    return texture(uTextures[14], vUV);
  else if(index == 15)
    return texture(uTextures[15], vUV);
  return vec4(1.f, 1.f, 1.f, 1.f);
}
