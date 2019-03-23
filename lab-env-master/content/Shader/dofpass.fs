#version 330                                                                        
			   
in vec2 TexCoord0;         
uniform sampler2D gColor;                                                                                 
uniform sampler2D gDepth;    

uniform vec2 sampleArray[12];
uniform float resX;                                                        
uniform float resY;                                                        

layout (location = 0) out vec4 outColor;  
  
const float GOLDEN_ANGLE = 2.39996323;
const float MAX_BLUR_SIZE = 10.0;
const float RAD_SCALE = 0.99; // Smaller = nicer blur, larger = faster

vec3 dof_stochastic()
{
	vec3 totalColor = texture(gColor, TexCoord0).rgb;
	float totalContribute = 1.0f;
	vec2 centerDepthVec = texture(gDepth, TexCoord0).rg;
	float centerSize = centerDepthVec.y * MAX_BLUR_SIZE;
	
	for(int i = 0; i < 12; i++)
	{
		//Sample coords
		vec2 sampleCoord = TexCoord0 + sampleArray[i] * centerSize;
		
		vec3 sampleColor = texture(gColor, sampleCoord).rgb;
		vec2 depthVec = texture(gDepth, sampleCoord).rg;
		float sampleDepth = depthVec.x;
		float sampleSize = depthVec.y;
		
		float sampleContribute = 1.0f; 
		
		if (sampleDepth > centerDepthVec.x)
		{
			sampleContribute = sampleSize;
		}
		
		totalColor += sampleColor * sampleContribute;
		totalContribute += sampleContribute;
	}
	
	vec3 color = totalColor / totalContribute;
	return color;
}

vec3 dof_gather_as_scatter()
{

	vec2 centerDepthVec = texture(gDepth, TexCoord0).rg;
	float centerDepth = centerDepthVec.x;
	float centerSize = centerDepthVec.y;
	vec3 color = texture(gColor, TexCoord0).rgb;
	float tot = 1.0;
	
	vec2 uPixelSize = vec2(1.0/resX, 1.0/resY);

	float radius = RAD_SCALE;
	for (float ang = 0.0; radius<MAX_BLUR_SIZE; ang += GOLDEN_ANGLE)
	{
		vec2 tc = TexCoord0 + vec2(cos(ang), sin(ang)) * uPixelSize * radius;

		vec3 sampleColor = texture(gColor, tc).rgb;
		vec2 depthVec = texture(gDepth, tc).rg;
		float sampleDepth = depthVec.x;
		float sampleSize = depthVec.y;
		if (sampleDepth > centerDepth)
		{
			sampleSize = clamp(sampleSize, 0.0, centerSize*2.0);
		}

		float m = smoothstep(radius-0.5, radius+0.5, sampleSize);
		color += mix(color/tot, sampleColor, m);
		tot += 1.0;

		radius += RAD_SCALE/radius;
		
	}
	return color / tot;
}


  
void main()                                                                                 
{   

	outColor.rgb = dof_stochastic();	
	//outColor.rgb = dof_gather_as_scatter();	

	outColor.a = 1.0f;	
}