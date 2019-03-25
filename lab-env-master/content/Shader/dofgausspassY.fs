#version 330                                                                        
                                                     
in vec2 TexCoord0;                                                                 

uniform sampler2D gColor;  
uniform sampler2D inDownSampled;  

layout(location = 0) out vec4 FragColor;    

void main()                                                                                 
{   
	FragColor = texture(gColor, TexCoord0);
		
}