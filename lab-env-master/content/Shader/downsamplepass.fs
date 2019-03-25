#version 330                                                                        
                                                     
in vec2 TexCoord0;     

uniform sampler2D inFragColor;   

uniform float resDownX;                                                        
uniform float resDownY;                                                               

layout(location = 0) out vec4 FragColor;    
  
void main()                                                                                 
{   

	vec2 scaledScreen = vec2(resDownX, resDownY); 
	vec3 col = texture(inFragColor, gl_FragCoord.xy * scaledScreen).xyz;
    FragColor = vec4( col.xyz, 1.0 );
	
		
}