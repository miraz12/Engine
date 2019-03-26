#version 330                                                                        
                                                     
in vec2 TexCoord0;     
                    

uniform vec2 sampleArrayX[7];
                                   
uniform sampler2D inDownSampled;  

uniform float resDownX;                                                        
uniform float resDownY;  

//Inner and outer weight thresholds
const vec4 threshInnner = vec4(0.1, 0.3, 0.5, -0.01);
const vec4 threshOuter = vec4(0.6, 0.7, 0.8, 0.9);

layout(location = 0) out vec4 FragColor;    

void main()                                                                                 
{
	vec2 scaledScreen = vec2(resDownX, resDownY); 
	vec2 tap0 = gl_FragCoord.xy * scaledScreen;
	vec2 tap1 = tap0 + sampleArrayX[1];
	vec2 tap2 = tap0 + sampleArrayX[2];
	vec2 tap3 = tap0 + sampleArrayX[3];
	vec2 tap4 = tap0 - sampleArrayX[1];
	vec2 tap5 = tap0 - sampleArrayX[2];
	vec2 tap6 = tap0 - sampleArrayX[3];
	vec4 s0, s1, s2, s3, s4, s5, s6;
	
	//Sample texture with taps for first 6 samples
	s0 = texture(inDownSampled, tap0);
	s1 = texture(inDownSampled, tap1);
	s2 = texture(inDownSampled, tap2);
	s3 = texture(inDownSampled, tap3);
	s4 = texture(inDownSampled, tap4);
	s5 = texture(inDownSampled, tap5);
	s6 = texture(inDownSampled, tap6);
	
	//calculate weights for inner positive samples
	vec4 weights4;
	weights4.x = clamp(s1.a - threshInnner.x, 0.0, 1.0);
	weights4.y = clamp(s2.a - threshInnner.y, 0.0, 1.0);
	weights4.z = clamp(s3.a - threshInnner.z, 0.0, 1.0);
	weights4.w = clamp(s0.a - threshInnner.w, 0.0, 1.0);
	
	//add positive inner sample color
	vec3 colorSum = (s1 * weights4.x + s2 * weights4.y + s3 * weights4.z + s0 * weights4.w).xyz;
	
	//add positive inner sample weights
	float totalWeight = dot(weights4, vec4(1.0f));
	
	//calculate weights for samples on other side of center point
	vec3 weight3; 
	weight3.x = clamp(s4.a - threshInnner.x, 0.0, 1.0); 
	weight3.y = clamp(s5.a - threshInnner.y, 0.0, 1.0); 
	weight3.z = clamp(s6.a - threshInnner.z, 0.0, 1.0); 
	
	//add negative inner sample color
	colorSum += (s4 * weight3.x + s5 * weight3.y + s6 * weight3.z).xyz;
		
	//add negative inner sample weights
	totalWeight += dot(weight3, vec3(1.0f));
	
	//Get outer taps
	vec2 tap7 = tap0 + sampleArrayX[4];
	vec2 tap8 = tap0 + sampleArrayX[5];
	vec2 tap9 = tap0 + sampleArrayX[6];
	vec2 tap10 = tap0 - sampleArrayX[4];
	vec2 tap11 = tap0 - sampleArrayX[5];
	vec2 tap12 = tap0 - sampleArrayX[6];
	
	//sample tap colors
	s0 = texture(inDownSampled, tap7);
	s1 = texture(inDownSampled, tap8);
	s2 = texture(inDownSampled, tap9);
	s3 = texture(inDownSampled, tap12);
	s4 = texture(inDownSampled, tap11);
	s5 = texture(inDownSampled, tap12);
	
	weight3.x = clamp(s0.a - threshOuter.x, 0.0, 1.0); 
	weight3.y = clamp(s1.a - threshOuter.y, 0.0, 1.0); 
	weight3.z = clamp(s2.a - threshOuter.z, 0.0, 1.0); 

	//add positive outer sample color
	colorSum += (s0 * weight3.x + s1 * weight3.y + s2 * weight3.z).xyz;
	
	//add positive outer sample weight
	totalWeight += dot(weight3, vec3(1.0f));

	weight3.x = clamp(s3.a - threshOuter.x, 0.0, 1.0); 
	weight3.y = clamp(s4.a - threshOuter.y, 0.0, 1.0); 
	weight3.z = clamp(s5.a - threshOuter.z, 0.0, 1.0); 

	//add negative outer sample color
	colorSum += (s3 * weight3.x + s4 * weight3.y + s5 * weight3.z).xyz;
	
	//add negative outer sample weight
	totalWeight += dot(weight3, vec3(1.0f));
	
	//divide by total weight
	colorSum /= totalWeight;
	

	// Color and weights sum output scaled (by 1/256)
	FragColor = vec4(colorSum, totalWeight) ;
}