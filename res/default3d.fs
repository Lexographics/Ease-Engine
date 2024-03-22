#version 300 es
precision mediump float;

layout(location = 0) out vec4 gAlbedo;
// layout(location = 1) out vec4 gPosition;
// layout(location = 2) out vec4 gNormal;
// layout(location = 3) out int gDrawID;


in vec2 vTexCoords;
in vec3 vNormal;
in vec3 vModelPos;


uniform int uDrawID;
// Material
uniform vec4 uMat_Albedo;
uniform sampler2D uMat_AlbedoTex;
uniform float uMat_Specular;
uniform sampler2D uMat_SpecularTex;


void main()
{
	vec4 albedo_tex = texture(uMat_AlbedoTex, vTexCoords).rgba;
	// float specular_tex = texture(uMat_SpecularTex, vTexCoords).r;
	
	// gAlbedo = albedo_tex * uMat_Albedo;
  gAlbedo = albedo_tex;
  // gAlbedo = vec4(1.0, 1.0, 1.0, 1.0);
	
	// gPosition = vec4(vModelPos, 1.0);
	// gNormal = vec4(normalize(vNormal), 1.0);

	// gDrawID = uDrawID;
}