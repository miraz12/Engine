#version 430

layout(location=0) in vec3 pos;
layout(location=1) in vec2 TexCoord;


out vec2 TexCoord0;

void main()
{
    TexCoord0 = TexCoord;
    gl_Position = vec4(pos, 1.0);
	
	
}
