#version 330                                                                        
                                                     
in vec2 TexCoord0;     

uniform sampler2D inFragColor;   
uniform sampler2D gDepth;  

uniform float resDownX;                                                        
uniform float resDownY;                                                               

layout(location = 0) out vec4 FragColor;    
  
void main()                                                                                 
{   
	vec2 scaledScreen = vec2(resDownX, resDownY); 
    FragColor = texture(inFragColor, gl_FragCoord.xy * scaledScreen);
	FragColor.a = texture(gDepth, gl_FragCoord.xy * scaledScreen).g;
}