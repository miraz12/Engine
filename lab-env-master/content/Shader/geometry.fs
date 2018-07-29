#version 330                                                                        
                			
uniform sampler2D DiffuseTextureSampler;
uniform sampler2D NormalTextureSampler;    
uniform sampler2D MaskTextureSampler;    
				
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
 
	vec3 Normal = CalcBumpedNormal();                                                                                           
                                                                                  
    FragColor = texture(DiffuseTextureSampler, TexCoord0);                             
}