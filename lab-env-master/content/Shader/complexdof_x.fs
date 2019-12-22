#version 330          

in vec2 TexCoord0;  
                                                              
const int MAX_COC = 8;
uniform vec2 kernelArray0[17];
uniform vec2 kernelArray1[17];

uniform sampler2D inFullRes;  
uniform sampler2D depth;  

uniform float pixelSizeX;                                                        
uniform float pixelSizeY;  


layout(location = 0) out vec4 colorOutRed;  
layout(location = 1) out vec4 colorOutGreen;  
layout(location = 2) out vec4 colorOutBlue;  


void main()  //x direction
{
    vec2 stepVal = vec2(pixelSizeX, pixelSizeY);
    
    vec4 colorRed = vec4(0,0,0,0);
    vec4 colorGreen = vec4(0,0,0,0);
    vec4 colorBlue = vec4(0,0,0,0);
    float filterRadius = texture(inFullRes, gl_FragCoord.xy*stepVal).a; //CoC Size saved in alpha
    for (int i=-MAX_COC; i <=MAX_COC; ++i)
    {
        vec2 sampleCoord = gl_FragCoord.xy *stepVal + stepVal*vec2(float(i),0.0)*filterRadius; 
		vec3 sampleColor = texture(inFullRes, sampleCoord).rgb;
        vec2 c0 = kernelArray0[i+MAX_COC];
		
		//complex multiplication with no imaginary color part.
        colorRed.xy += sampleColor.r * c0;
		colorGreen.xy += sampleColor.g * c0;
		colorBlue.xy += sampleColor.b * c0;


        
    }
    colorOutRed = colorRed;
    colorOutGreen = colorGreen;
    colorOutBlue = colorBlue;
   
}

