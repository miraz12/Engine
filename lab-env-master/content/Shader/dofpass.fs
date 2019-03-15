#version 330                                                                        
                                                     
in vec2 TexCoord0;         
uniform sampler2D gColor;                                                                                 
uniform sampler2D gDepth;    

uniform float inFocusPoint;                                                        
uniform float inFocusScale;                                                        
                                                                             

layout (location = 4) out vec4 outColor;  
  
float near = 0.1; 
float far  = 100.0; 
  
const float GOLDEN_ANGLE = 2.39996323;
const float MAX_BLUR_SIZE = 20.0;
const float RAD_SCALE = 0.5; // Smaller = nicer blur, larger = faster

float getBlurSize(float depth, float focusPoint, float focusScale)
{
 float coc = clamp((1.0 / focusPoint - 1.0 / depth)*focusScale, -1.0, 1.0);
 return abs(coc) * MAX_BLUR_SIZE;
}
  
void main()                                                                                 
{   
	float focusPoint = inFocusPoint;
	float focusScale = inFocusScale;
	
	float centerDepth = texture(gDepth, TexCoord0).r * far;
	float centerSize = getBlurSize(centerDepth, focusPoint, focusScale);
	vec3 color = texture(gColor, TexCoord0).rgb;
	float tot = 1.0;
	
	vec2 uPixelSize = vec2(1.0/1024.0, 1.0/768.0);
	

	float radius = RAD_SCALE;
	for (float ang = 0.0; radius<MAX_BLUR_SIZE; ang += GOLDEN_ANGLE)
	{
		vec2 tc = TexCoord0 + vec2(cos(ang), sin(ang)) * uPixelSize * radius;

		vec3 sampleColor = texture(gColor, tc).rgb;
		float sampleDepth = texture(gDepth, tc).r * far;
		float sampleSize = getBlurSize(sampleDepth, focusPoint, focusScale);
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