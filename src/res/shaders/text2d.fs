#version 300 es
precision mediump float;

layout (location = 0) out vec4 color;
layout (location = 1) out uint drawId;

in vec4 vColor;
in vec2 vUV;
in float vTexture;
flat in uint vDrawID;

uniform sampler2D uTextures[32];


vec4 getTexture();

void main() {
  color = vec4(1.0, 1.0, 1.0, getTexture().r) * vColor;
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
  else if(index == 16)
    return texture(uTextures[16], vUV);
  else if(index == 17)
    return texture(uTextures[17], vUV);
  else if(index == 18)
    return texture(uTextures[18], vUV);
  else if(index == 19)
    return texture(uTextures[19], vUV);
  else if(index == 20)
    return texture(uTextures[20], vUV);
  else if(index == 21)
    return texture(uTextures[21], vUV);
  else if(index == 22)
    return texture(uTextures[22], vUV);
  else if(index == 23)
    return texture(uTextures[23], vUV);
  else if(index == 24)
    return texture(uTextures[24], vUV);
  else if(index == 25)
    return texture(uTextures[25], vUV);
  else if(index == 26)
    return texture(uTextures[26], vUV);
  else if(index == 27)
    return texture(uTextures[27], vUV);
  else if(index == 28)
    return texture(uTextures[28], vUV);
  else if(index == 29)
    return texture(uTextures[29], vUV);
  else if(index == 30)
    return texture(uTextures[30], vUV);
  else if(index == 31)
    return texture(uTextures[31], vUV);
  return vec4(1.f, 1.f, 1.f, 1.f);
}
