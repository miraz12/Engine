#version 330                                                                        
                
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
				
in vec2 TexCoord0;                                                                 
in vec3 Normal0;                                                                 
in vec3 WorldPos0;                                                                
in vec3 Tangent0;                                                                          

out vec4 FragColor;                                                                 
  
vec3 CalcBumpedNormal()
{
    vec3 Normal = normalize(Normal0);
    vec3 Tangent = normalize(Tangent0);
    Tangent = normalize(Tangent - dot(Tangent, Normal) * Normal);
    vec3 Bitangent = cross(Tangent, Normal);
    vec3 BumpMapNormal = texture(NormalTextureSampler, TexCoord0).xyz;
    BumpMapNormal = 2.0 * BumpMapNormal - vec3(1.0, 1.0, 1.0);
    vec3 NewNormal;
    mat3 TBN = mat3(Tangent, Bitangent, Normal);
    NewNormal = TBN * BumpMapNormal;
    NewNormal = normalize(NewNormal);
    return NewNormal;
}
				
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

	//vec4 color = texture(DiffuseTextureSampler, TexCoord0);
    //vec4 mask  = texture(MaskTextureSampler, TexCoord0);	
                                                                                            
    FragColor = texture(DiffuseTextureSampler, TexCoord0) * TotalLight;                             
}