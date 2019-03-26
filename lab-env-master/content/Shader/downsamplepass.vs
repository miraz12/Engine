#version 430

layout(location=0) in vec3 pos;
layout(location=1) in vec2 TexCoord;

uniform float resDownX;                                                        
uniform float resDownY; 

out vec2 TexCoord0;



void main()
{
	
	TexCoord0 = TexCoord*0.5f;
    gl_Position = vec4(pos, 1.0);
	TexCoord0 = pos.xy;
}
