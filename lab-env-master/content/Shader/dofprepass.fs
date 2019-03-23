#version 330                                                                        

in vec2 TexCoord0;                                                                                        
uniform sampler2D gDepth;    
 
uniform float inDistToFocus;                                                        
uniform float inFocalLen;                                                        
uniform float inAperture;                                                        


layout (location = 1) out vec4 outDepth;  


const float MaxCocSize = 10.0;

//Focus scale is based on focal lenngth and aperture
float getBlurSize(float depth, float focusPoint, float focusScale)
{
	float coc = clamp((1.0 / focusPoint - 1.0 / depth)*focusScale, -1.0, 1.0);
	return abs(coc) * MaxCocSize;
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
	float distToFocus = inDistToFocus;
	float focalLen = inFocalLen;
	float aperture = inAperture;
	float depth = texture(gDepth, TexCoord0).r;
	
	//float centerSize = getBlurSize(centerDepth, focusPoint, focusScale);
	float centerSize = getBlurSizePhysical(depth, distToFocus, focalLen, aperture);
	outDepth.g = centerSize;
}