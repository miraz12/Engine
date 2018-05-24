#version 330 core

in vec2 UV;
in vec4 particlecolor;

out vec4 color;

uniform sampler2D particleTexture;

void main(){
	// Output color = color of the texture at the specified UV
	color = texture( particleTexture, UV ) * particlecolor;

}