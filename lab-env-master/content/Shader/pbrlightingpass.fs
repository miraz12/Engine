#version 330                                                                        
                                                                                                                                     
                                                                                    
in vec2 TexCoord0;                                                              

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedoSpec;                                                                                                                                           
uniform sampler2D gDepth;                                                                                                                                           

layout (location = 0) out vec4 gColor;
    
								
uniform vec3 cameraPos;                                                                  
uniform float gMatSpecularIntensity;                                                        
uniform float gSpecularPower;     

float DistributionGGX(vec3 N, vec3 H, float roughness);
float GeometrySchlickGGX(float NdotV, float roughness);
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness);
vec3 fresnelSchlick(float cosTheta, vec3 F0);

const float PI = 3.14159265359;

const vec3 lighPos[3] = vec3[3](
vec3(0, 100, -180.f),
vec3(100, 1000, 0.f),
vec3(-950, 100, 0.f)
);

void main()                                                                                 
{   

	vec3 WorldPos = texture(gPosition, TexCoord0).rgb;
    vec3 N = texture(gNormal, TexCoord0).rgb;
    vec3 albedo =  pow(texture(gAlbedoSpec, TexCoord0).rgb, vec3(2.2));
	float Metallic = texture(gAlbedoSpec, TexCoord0).a;
	float Roughness = texture(gNormal, TexCoord0).a;
	
	vec3 V = normalize(cameraPos - WorldPos);
	
	// calculate reflectance at normal incidence; if dia-electric (like plastic) use F0 
    // of 0.04 and if it's a metal, use the albedo color as F0 (metallic workflow)    
	vec3 F0 = vec3(0.04); 
    F0 = mix(F0, albedo, Metallic);
	
	//Hard coded lights for now.
	vec3 lightColor = vec3(300000.0f, 300000.0f, 300000.0f);
	
	
	// reflectance equation
    vec3 Lo = vec3(0.0);
	for(int i = 0; i < 3; ++i) 
    {

		
		// calculate per-light radiance
		vec3 L = normalize(lighPos[i] - WorldPos);
		vec3 H = normalize(V + L);
		float distance = length(lighPos[i] - WorldPos);
		float attenuation = 1.0 / (distance * distance);
		vec3 radiance = lightColor * attenuation;

		// Cook-Torrance BRDF
		float NDF = DistributionGGX(N, H, Roughness);   
		float G   = GeometrySmith(N, V, L, Roughness);      
		vec3 F    = fresnelSchlick(max(dot(H, V), 0.0), F0);
		   
		vec3 nominator    = NDF * G * F; 
		float denominator = 4 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.001; // 0.001 to prevent divide by zero.
		vec3 specular = nominator / denominator;
		
		// kS is equal to Fresnel
		vec3 kS = F;
		// for energy conservation, the diffuse and specular light can't
		// be above 1.0 (unless the surface emits light); to preserve this
		// relationship the diffuse component (kD) should equal 1.0 - kS.
		vec3 kD = vec3(1.0) - kS;
		// multiply kD by the inverse metalness such that only non-metals 
		// have diffuse lighting, or a linear blend if partly metal (pure metals
		// have no diffuse light).
		kD *= 1.0 - Metallic;	  

		// scale light by NdotL
		float NdotL = max(dot(N, L), 0.0);        

		// add to outgoing radiance Lo
		Lo += (kD * albedo / PI + specular) * radiance * NdotL;  // note that we already multiplied the BRDF by the Fresnel (kS) so we won't multiply by kS again
	}
	
	// ambient lighting (note that the next IBL tutorial will replace 
    // this ambient lighting with environment lighting).
    vec3 ambient = vec3(0.03) * albedo;
    
    vec3 color = ambient + Lo;
	
	// HDR tonemapping
    color = albedo / (albedo + vec3(1.0));
    // gamma correct
    color = pow(albedo, vec3(1.0/2.2)); 
	
	gColor.rgb = color;
    gColor.a = texture(gDepth, TexCoord0).g; ;  
		
}

// ----------------------------------------------------------------------------
float DistributionGGX(vec3 N, vec3 H, float roughness)
{
    float a = roughness*roughness;
    float a2 = a*a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH*NdotH;

    float nom   = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;

    return nom / denom;
}
// ----------------------------------------------------------------------------
float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r*r) / 8.0;

    float nom   = NdotV;
    float denom = NdotV * (1.0 - k) + k;

    return nom / denom;
}
// ----------------------------------------------------------------------------
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2 = GeometrySchlickGGX(NdotV, roughness);
    float ggx1 = GeometrySchlickGGX(NdotL, roughness);

    return ggx1 * ggx2;
}
// ----------------------------------------------------------------------------
vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}
// ----------------------------------------------------------------------------