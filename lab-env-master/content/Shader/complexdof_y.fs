#version 330          

in vec2 TexCoord0;  
                                                              
const int MAX_COC = 8;
uniform vec2 kernelArray0[17];
uniform vec2 kernelArray1[17];

uniform sampler2D inFullRes;    
uniform sampler2D colorOutRed;  
uniform sampler2D colorOutGreen;  
uniform sampler2D colorOutBlue;  

uniform float pixelSizeX;                                                        
uniform float pixelSizeY;
uniform int nrComp;

uniform vec2 kernellWeights0;
uniform vec2 kernellWeights1;

layout(location = 3) out vec4 FragColor;    


//(Pr+Pi)*(Qr+Qi) = (Pr*Qr+Pr*Qi+Pi*Qr-Pi*Qi)
vec2 multComplex(vec2 p, vec2 q)
{
    return vec2(p.x*q.x-p.y*q.y, p.x*q.y+p.y*q.x);
}

void main()  //x direction
{
    vec2 stepVal = vec2(pixelSizeX, pixelSizeY);
    
    vec4 colorRed = vec4(0,0,0,0);
    vec4 colorGreen = vec4(0,0,0,0);
    vec4 colorBlue = vec4(0,0,0,0);
    float filterRadius = texture(inFullRes, gl_FragCoord.xy*stepVal).a; //CoC Size saved in alpha
    for (int i=-MAX_COC; i <=MAX_COC; ++i)
    {
        vec2 sampleCoord = gl_FragCoord.xy *stepVal + stepVal*vec2(0.0,float(i))*filterRadius; //stepVal*i can be precalculated on cpu and sent as array to save calulcations for each framgent.
        vec4 imageTexelR = texture(colorOutRed, sampleCoord);
        vec4 imageTexelG = texture(colorOutGreen, sampleCoord);
        vec4 imageTexelB = texture(colorOutBlue, sampleCoord);
		
        vec2 c0 = kernelArray0[i+MAX_COC];

		
        colorRed.xy += multComplex(imageTexelR.xy,c0);
		colorGreen.xy += multComplex(imageTexelG.xy,c0);
		colorBlue.xy += multComplex(imageTexelB.xy,c0);
        
		
		if(nrComp == 2)
		{
		    vec2 c1 = kernelArray1[i+MAX_COC];
			colorRed.zw += multComplex(imageTexelR.zw,c1);
			colorGreen.zw += multComplex(imageTexelG.zw,c1);
			colorBlue.zw += multComplex(imageTexelB.zw,c1);
		}
        
    }
	float redChannel   = dot(colorRed.xy, kernellWeights0);
    float greenChannel = dot(colorGreen.xy, kernellWeights0);
    float blueChannel  = dot(colorBlue.xy, kernellWeights0);
	
	if(nrComp == 2)
	{
		redChannel   += dot(colorRed.zw, kernellWeights1);
		greenChannel += dot(colorGreen.zw, kernellWeights1);
		blueChannel  += dot(colorBlue.zw, kernellWeights1);
	}
	
	FragColor = vec4(vec3(redChannel,greenChannel,blueChannel),1.0);   
    
}
