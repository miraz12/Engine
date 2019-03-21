#version 330                                                                        

in vec2 TexCoord0;                                                                                        
uniform sampler2D gDepth;    

uniform float inFocusPoint;                                                        
uniform float inFocusScale;                                                        
uniform float inFar; 

layout (location = 3) out vec4 outDepth;  

float far  = inFar; 
const float maxBlur = 5.0;


float getBlurSize(float depth, float focusPoint, float focusScale)
{
 float coc = clamp((1.0 / focusPoint - 1.0 / depth)*focusScale, -1.0, 1.0);
 return abs(coc) * maxBlur;
}
  
void main()                                                                                 
{   
	float focusPoint = inFocusPoint;
	float focusScale = inFocusScale;
	
	float centerDepth = texture(gDepth, TexCoord0).r * far;
	float centerSize = getBlurSize(centerDepth, focusPoint, focusScale);
	outDepth.g = centerSize;
}