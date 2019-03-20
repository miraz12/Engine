#version 330                                                                        
                                                     
in vec2 TexCoord0; 
uniform sampler2D gColor;                                                                

layout(location = 0) out vec4 FragColor;    
  
void main()                                                                                 
{   
	FragColor.rgb = texture(gColor, TexCoord0).xyz;
	FragColor.a = 1.0f;
		
}