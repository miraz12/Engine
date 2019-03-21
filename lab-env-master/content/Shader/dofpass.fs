#version 330                                                                        
                                                     
in vec2 TexCoord0;         
uniform sampler2D gColor;                                                                                 
uniform sampler2D gDepth;    

uniform float inFocusPoint;                                                        
uniform float inFocusScale;  
uniform float inFar;
                                                      
uniform float resX;                                                        
uniform float resY;                                                        
                                                                             

layout (location = 4) out vec4 outColor;  
  
float far  = inFar; 
  
const float GOLDEN_ANGLE = 2.39996323;
const float MAX_BLUR_SIZE = 5.0;
const float RAD_SCALE = 0.99; // Smaller = nicer blur, larger = faster

  
void main()                                                                                 
{   
	float focusPoint = inFocusPoint;
	float focusScale = inFocusScale;
	
	vec2 centerDepthVec = texture(gDepth, TexCoord0).rg;
	float centerDepth = centerDepthVec.x * far;
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
		float sampleDepth = depthVec.x * far;
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
	outColor.rgb = color / tot;
	
}