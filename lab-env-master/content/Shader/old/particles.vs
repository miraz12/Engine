#version 330 core

layout(location = 4) in vec3 squareVertices;
layout(location = 5) in vec4 xyzs; // Position of the center of the particle and size of the square
layout(location = 6) in vec4 color; 

out vec2 UV;
out vec4 particlecolor;

uniform vec3 CameraRight_worldspace;
uniform vec3 CameraUp_worldspace;
uniform mat4 VP;

void main()
{
	float particleSize = xyzs.w; // zize is stored in fourth of position vector
	vec3 particleCenter_wordspace = xyzs.xyz;
	
	vec3 vertexPosition_worldspace = 
		particleCenter_wordspace
		+ CameraRight_worldspace * squareVertices.x * particleSize
		+ CameraUp_worldspace * squareVertices.y * particleSize;

	// Output position of the vertex
	gl_Position = VP * vec4(vertexPosition_worldspace, 1.0f);

	// UV of the vertex. No special space for this one.
	UV = squareVertices.xy + vec2(0.5, 0.5);
	particlecolor = color;
}

