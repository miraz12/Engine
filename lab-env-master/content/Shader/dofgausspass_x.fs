#version 330                                                                        
                                                     
in vec2 TexCoord0;     
                    

uniform vec2 sampleArrayX[7];
                                   
uniform sampler2D inDownSampled;  

uniform float resDownX;                                                        
uniform float resDownY;  

//Inner and outer weight thresholds
const vec4 threshInnner = vec4(0.1, 0.3, 0.5, -0.01);
const vec3 threshOuter = vec3(0.6, 0.7, 0.8);

layout(location = 1) out vec4 FragColorY;    

float g_fSigma = 5.0f;

float CalcGaussianWeight(int iSamplePoint)
{
	float g = 1.0f / sqrt(2.0f * 3.14159 * g_fSigma * g_fSigma);  
	return (g * exp(-(iSamplePoint * iSamplePoint) / (2 * g_fSigma * g_fSigma)));
}

void main()                                                                                 
{
	vec2 scaledScreen = vec2(resDownX, resDownY); 
	vec2 tap0 = gl_FragCoord.xy * scaledScreen;

	
	vec3 totalColor;// = texture(gColor, tap0).rgb;
	float centerSize = texture(inDownSampled, gl_FragCoord.xy*scaledScreen).a ;

	int iRadius = 25;
	
	for (int i = -iRadius; i < iRadius; i++)
	{
		
		float fWeight = CalcGaussianWeight(i);
		vec2 sampleCoord = tap0 + scaledScreen*vec2(float(i),0.0)*centerSize; 		
		vec3 sampleColor = texture(inDownSampled, sampleCoord).rgb;
		totalColor += sampleColor * fWeight;
	}
	

	// Color and weights sum 
	FragColorY = vec4(totalColor, 1.0f);
}


/*


float g_fSigma = 1.0f;

float CalcGaussianWeight(int iSamplePoint)
{
	float g = 1.0f / sqrt(2.0f * 3.14159 * g_fSigma * g_fSigma);  
	return (g * exp(-(iSamplePoint * iSamplePoint) / (2 * g_fSigma * g_fSigma)));
}

void main()                                                                                 
{
	vec2 scaledScreen = vec2(resDownX, resDownY); 
	vec2 tap0 = gl_FragCoord.xy * scaledScreen;

	
	vec3 totalColor;// = texture(gColor, tap0).rgb;
	float centerSize = texture(inDownSampled, gl_FragCoord.xy*scaledScreen).a ;

	int iRadius = 64;
	
	for (int i = -iRadius; i < iRadius; i++)
	{
		
		float fWeight = CalcGaussianWeight(i);
		vec2 sampleCoord = tap0 + scaledScreen*vec2(float(i),0.0)*centerSize; 		
		vec3 sampleColor = texture(inDownSampled, sampleCoord).rgb;
		totalColor += sampleColor * fWeight;
	}
	

	// Color and weights sum 
	FragColorY = vec4(totalColor, 1.0f);
*/