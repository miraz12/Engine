#version 330                                                                        
                                                                                                      
in vec2 TexCoord0;     
                    
uniform vec2 sampleArrayY[7];
                                   
uniform sampler2D inDownSampled;  

uniform float resDownX;                                                        
uniform float resDownY;  

//Inner and outer weight thresholds
const vec4 threshInnner = vec4(0.08f, 0.075f, 0.07f, 0.1f);
const vec4 threshOuter = vec4(0.065, 0.06, 0.055f, 0.05f);

layout(location = 0) out vec4 FragColor;    

void main()                                                                                 
{   
	
	vec2 scaledScreen = vec2(resDownX, resDownY); 
	vec2 tap0 = gl_FragCoord.xy * scaledScreen;
	vec2 tap1 = tap0 + sampleArrayY[1];
	vec2 tap2 = tap0 + sampleArrayY[2];
	vec2 tap3 = tap0 + sampleArrayY[3];
	vec2 tap4 = tap0 - sampleArrayY[1];
	vec2 tap5 = tap0 - sampleArrayY[2];
	vec2 tap6 = tap0 - sampleArrayY[3];
	vec4 s0, s1, s2, s3, s4, s5, s6;
	
	//Sample texture with taps for first 6 samples
	s0 = texture(inDownSampled, tap0);
	s1 = texture(inDownSampled, tap1);
	s2 = texture(inDownSampled, tap2);
	s3 = texture(inDownSampled, tap3);
	s4 = texture(inDownSampled, tap4);
	s5 = texture(inDownSampled, tap5);
	s6 = texture(inDownSampled, tap6);
	
	s0.rgb = s0.rgb * s0.a;
	s1.rgb = s1.rgb * s1.a;
	s2.rgb = s2.rgb * s2.a;
	s3.rgb = s3.rgb * s3.a;
	s4.rgb = s4.rgb * s4.a;
	s5.rgb = s5.rgb * s5.a;
	s6.rgb = s6.rgb * s6.a;
	
	vec4 weights4 = s0 * threshInnner.w +
			 (s1 + s4) * threshInnner.x +
			 (s2 + s5) * threshInnner.y +
			 (s3 + s6) * threshInnner.z;
		
	//Get outer taps
	vec2 tap7 = tap0 + sampleArrayY[4];
	vec2 tap8 = tap0 + sampleArrayY[5];
	vec2 tap9 = tap0 + sampleArrayY[6];
	vec2 tap10 = tap0 - sampleArrayY[4];
	vec2 tap11 = tap0 - sampleArrayY[5];
	vec2 tap12 = tap0 - sampleArrayY[6];
	
	//sample tap colors
	s0 = texture(inDownSampled, tap7);
	s1 = texture(inDownSampled, tap8);
	s2 = texture(inDownSampled, tap9);
	s3 = texture(inDownSampled, tap12);
	s4 = texture(inDownSampled, tap11);
	s5 = texture(inDownSampled, tap12);
	
	//Modulate color values by weights
	s0.rgb = s0.rgb * s0.a;
	s1.rgb = s1.rgb * s1.a;
	s2.rgb = s2.rgb * s2.a;
	s3.rgb = s3.rgb * s3.a;
	s4.rgb = s4.rgb * s4.a;
	s5.rgb = s5.rgb * s5.a;

	weights4 += (s0 + s3) * threshOuter.x +
				(s1 + s4) * threshOuter.y +
				(s2 + s5) * threshOuter.z;
				
	weights4.rgb /= weights4.a;

	FragColor = weights4;
	
}