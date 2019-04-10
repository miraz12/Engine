#version 330                                                                        

layout (location = 1) out vec4 gNormal;
layout (location = 2) out vec4 gAlbedoSpec;
layout (location = 3) out vec2 gDepth;
layout (location = 4) out vec4 gPosition;

uniform float inDistToFocus;                                                        
uniform float inFocalLen;                                                        
uniform float inAperture;         

uniform sampler2D DiffuseTextureSampler;
uniform sampler2D NormalTextureSampler;     
				
in vec2 TexCoord0;                                                                 
in vec3 Normal0;                                                                 
in vec3 WorldPos0;                                                                
in vec3 Tangent0;                                                                                                                                        
in float Depth0;         
                                                                                                                          
  
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

//A calculation of CoC size based on lens properties. current depth, distance from plane in focus, lense focal lenngth, lens diameter CoC = |D * f(zfocus - z) / zfocus * (z - f)|
float getBlurSizePhysical(float depth, float zfocus, float focallen, float aperture)
{
	float diaLens = focallen/aperture;
 	float coc = abs(diaLens * focallen * (zfocus - depth) / (zfocus * (depth - focallen)));
	return clamp(coc , 0.0, 1.0);
}

				
void main()                                                                                 
{   
	gAlbedoSpec = texture(DiffuseTextureSampler, TexCoord0);
	gPosition = vec4(WorldPos0, 1.0f);
	gNormal = vec4(CalcBumpedNormal(),1.0);
	gDepth.r = (-Depth0);
	
	float distToFocus = inDistToFocus;
	float focalLen = inFocalLen;
	float aperture = inAperture;
	
	//float centerSize = getBlurSize(centerDepth, focusPoint, focusScale);
	float centerSize = getBlurSizePhysical(gDepth.r, distToFocus, focalLen, aperture);
	//float centerSize = clamp(abs(gDepth.r - inDistToFocus) / inFocalLen , 0.0f, 1.0f);
	gDepth.g = centerSize;

	if(gAlbedoSpec.a < 0.1) 
	{
		discard;
	}
}