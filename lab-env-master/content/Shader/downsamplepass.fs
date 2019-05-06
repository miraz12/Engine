#version 330                                                                        
                                                     
in vec2 TexCoord0;     

uniform sampler2D inFullRes;   
uniform sampler2D inFullResDepth;     

uniform float resDownX;                                                        
uniform float resDownY;                                                               

layout(location = 0) out vec4 FragColor;    
layout(location = 3) out vec4 depth;    
  
void main()                                                                                 
{   
	vec2 scaledScreen = vec2(resDownX, resDownY); 
    
	FragColor = texture(inFullRes, gl_FragCoord.xy * scaledScreen);
    depth = texture(inFullResDepth, gl_FragCoord.xy * scaledScreen);
}