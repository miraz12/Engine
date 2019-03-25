#version 430

layout(location=0) in vec3 pos;
layout(location=1) in vec2 TexCoord;

uniform vec2 sampleArrayX[7];

out vec2 TexCoord0;
out vec2 tap0;
out vec2 tap1;
out vec2 tap2;
out vec2 tap3;
out vec2 tap4;
out vec2 tap5;
out vec2 tap6;


void main()
{
    TexCoord0 = TexCoord;
    gl_Position = vec4(pos, 1.0);
	tap0 = TexCoord0;
	tap1 = TexCoord0 + sampleArrayX[1];
	tap2 = TexCoord0 + sampleArrayX[2];
	tap3 = TexCoord0 + sampleArrayX[3];
	tap4 = TexCoord0 - sampleArrayX[1];
	tap5 = TexCoord0 - sampleArrayX[2];
	tap6 = TexCoord0 - sampleArrayX[3];
	
}
