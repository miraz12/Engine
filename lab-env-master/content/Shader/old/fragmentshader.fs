#version 330   
#extension GL_ARB_shading_language_include : require

#include </lighting.fs>                                                                   
                                                                                    
const int MAX_POINT_LIGHTS = 2;                                                     
const int MAX_SPOT_LIGHTS = 2;                                                      
                                                                                    
in vec2 TexCoord0;                                                                 
in vec3 Normal0;                                                                 
in vec3 WorldPos0;                                                                
in vec3 Tangent0;    
in vec4 ColorIn;                                                                             

out vec4 FragColor;                                                                 
   

void main()                                                                                 
{   
 
	vec3 Normal;                                                                                        

	Normal = CalcBumpedNormal();
	
    vec4 TotalLight = CalcDirectionalLight(Normal);                                         
                                                                                            
    for (int i = 0 ; i < gNumPointLights; i++) {                                           
        TotalLight += CalcPointLight(gPointLights[i], Normal);                              
    }                                                                                       
                                                                                            
    for (int i = 0 ; i < gNumSpotLights ; i++) {                                            
        TotalLight += CalcSpotLight(gSpotLights[i], Normal);                                
    }                                                                                       
                                                                                            
    FragColor = texture2D(DiffuseTextureSampler, TexCoord0.xy) * TotalLight;                             
}