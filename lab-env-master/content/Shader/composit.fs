#version 330                                                                        
                                                     
in vec2 TexCoord0;                
  
uniform sampler2D inFullRes;    
uniform sampler2D inDownSampled;  

layout(location = 0) out vec4 FragColor;    
  
void main()                                                                                 
{   

	vec4 blurr = texture(inDownSampled, TexCoord0);
	vec4 full = texture(inFullRes, TexCoord0);
	FragColor = mix(full, blurr, full.a);
	FragColor.a = 1.0f;
}