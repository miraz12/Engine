#version 330                                                                        

in vec2 TexCoord0;                                                                                        
uniform sampler2D gDepth;    

uniform float inFocusPoint;                                                        
uniform float inFocusScale;                                                        
uniform float inFar; 

layout (location = 3) out vec4 outDepth;  

float far  = inFar; 

const float MaxCocSize = 5.0;

//Focus scale is based on focal lenngth and apture
float getBlurSize(float depth, float focusPoint, float focusScale)
{
	float coc = clamp((1.0 / focusPoint - 1.0 / depth)*focusScale, -1.0, 1.0);
	return abs(coc) * MaxCocSize;
}

//A calculation of CoC size based on lens properties. current depth, distance from plane in focus, lense focal lenngth, lens diameter CoC = |D * f(zfocus - z) / zfocus * (z - f)|
float getBlurSizePhysical(float depth, float zfocus, float focallen, float dialens)
{
 	float coc = abs(dialens * focallen * (zfocus - depth) / (zfocus * (depth - focallen)));
	return clamp(coc , 0.0, 1.0);
}
  
void main()                                                                                 
{   
	float focusPoint = inFocusPoint;
	float focusScale = inFocusScale;
		
	float centerDepth = texture(gDepth, TexCoord0).r;
	
	float zfocus = inFocusPoint + centerDepth; //distance from object to plane in focus
	//float centerSize = getBlurSize(centerDepth, focusPoint, focusScale);
	float centerSize = getBlurSizePhysical(centerDepth, inFocusPoint, focusScale, 1.0f);
	outDepth.g = centerSize;
}