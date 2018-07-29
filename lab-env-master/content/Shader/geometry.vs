#version 430
uniform mat4 projection;
uniform mat4 model;
uniform mat4 view;

out vec2 TexCoord0;
out vec3 Normal0;
out vec3 WorldPos0;
out vec3 Tangent0;
out vec4 ColorIn;

layout(location=0) in vec3 pos;
layout(location=1) in vec2 TexCoord;
layout(location=2) in vec3 normal;
layout(location=3) in vec3 tangents;

void main()
{
	gl_Position = ( projection * view * model) * vec4(pos, 1.0);

	TexCoord0 = TexCoord;
	Normal0 = (model * vec4(normal, 0.0)).xyz;
	Tangent0 = (model * vec4(tangents, 0.0)).xyz;
	WorldPos0   = (model * vec4(pos, 1.0)).xyz;  
}
