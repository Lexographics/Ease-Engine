#version 300 es
precision mediump float;

layout (location = 0) out vec4 color;

in vec2 vUV;

uniform sampler2D uTexture;

void main() {
  color = texture(uTexture, vUV);

  if(color.a < 0.1f)
    discard;
}
