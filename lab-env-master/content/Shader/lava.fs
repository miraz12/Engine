#version 330                                                                        


uniform float iTime;

in vec2 TexCoord0; 

out vec4 FragColor;

vec2 uv;  
  

/*
Lava texture based on Fractal Brownian motion as described by
https://thebookofshaders.com/13/

The fbm is thresholded to generate floating rocks.

Colorsblending modes (for glow etc) are based on their implementations in Adobe Photoshop.
*/

//-------------------------------------------------
// Define colorscheme
//-------------------------------------------------
vec3 orange = vec3(0.65, 0.37, 0);
vec3 middleRed1 = vec3(.5882, .2451, .1922);
vec3 middleRed2 = vec3(.8196, .3451, .3176);

//-------------------------------------------------
// Noise & random-functions
//-------------------------------------------------
float rand(vec2 co, float seed){
    //https://www.shadertoy.com/view/ltB3zD
    float phi = 1.61803398874989484820459 * 00000.1; // Golden Ratio   
	float pi  = 3.14159265358979323846264 * 00000.1; // PI
	float sq2 = 1.41421356237309504880169 * 10000.0; // Square Root of Two
    return fract(sin(dot(co*(seed+phi), vec2(phi, pi)))*sq2);
}

float rand(vec2 st){
    //https://thebookofshaders.com/13/
    return fract(sin(dot(st,
                         vec2(12.9898,78.233)))*
        43758.5453123);
}

float noise(vec2 x, float seed) {
    //https://thebookofshaders.com/11/ 
    //(in turn based on https://www.shadertoy.com/view/4dS3Wd)
    vec2 i = floor(x);
    vec2 f = fract(x);
    float y = rand(vec2(0.5, 0.5), 0.5);
    float a = rand(i, seed);
    float b = rand(i + vec2(1.0, 0.0), seed);
    float c = rand(i + vec2(0.0, 1.0), seed);
    float d = rand(i + vec2(1.0, 1.0), seed);
    vec2 u = smoothstep(0.0, 1.0, f);
    return mix(a, b, u.x) +
            (c - a)* u.y * (1.0 - u.x) +
            (d - b) * u.x * u.y;
}

float noise(vec2 x) {
    //https://thebookofshaders.com/11/
    vec2 i = floor(x);
    vec2 f = fract(x);
    float y = rand(vec2(0.5, 0.5), 0.5);
    float a = rand(i);
    float b = rand(i + vec2(1.0, 0.0));
    float c = rand(i + vec2(0.0, 1.0));
    float d = rand(i + vec2(1.0, 1.0));
    vec2 u = smoothstep(0.0, 1.0, f);
    return mix(a, b, u.x) +
            (c - a)* u.y * (1.0 - u.x) +
            (d - b) * u.x * u.y;
}

float fbm(vec2 st) {
    //https://thebookofshaders.com/13/
    float value = 0.0;
    float amplitude = .5;
    float frequency = 0.;
    // Loop of octaves
    for (int i = 0; i < 10; i++) {
        value += amplitude * noise(st);
        st *= 2.2;
        amplitude *= .5;
    }
    return value;
}
//-------------------------------------------------
// Some useful functions for changing colors etc
//-------------------------------------------------
vec3 rgbToYuv(vec3 rgb) {
    mat3 transformation = mat3(0.299, 0.587, 0.114, 
                               -0.14713, -0.28886, 0.436,
                              0.615, -0.51499, -0.10001);
    return transformation * rgb;
}
vec3 yuvToRgb(vec3 yuv) {
    mat3 transformation = mat3(1.0, 0.0, 1.13983, 
                               1.0, -0.39465, -0.5806,
                              1.0, 2.03211, 0.0);
    return transformation * yuv;
}
vec3 colorDodge(vec3 color) {
    //https://photoblogstop.com/photoshop/photoshop-blend-modes-explained
    vec3 dodgeBy = vec3(.5, .5, .5);
    dodgeBy = vec3(1.0, 1.0, 1.0) - dodgeBy;
    color /= dodgeBy;
    return color;
}
vec3 colorBurn(vec3 color, vec3 burnBy) {
    color = vec3(1.0, 1.0, 1.0) - color;
    color /= burnBy;
    return vec3(1.0, 1.0, 1.0) - color;
}
vec3 colorBurn(vec3 color) {
    return colorBurn(color, vec3(.6));
}
vec3 colorScreen(vec3 bgcolor, vec3 addcolor) {
    vec3 onevector = vec3(1.0, 1.0, 1.0);
    bgcolor = onevector - bgcolor;
    addcolor = onevector - bgcolor;
    return onevector - (bgcolor * addcolor);
}
vec3 colorMultiply(vec3 bgcolor, vec3 addcolor) {
    return bgcolor * addcolor;
}
vec3 colorOverlay(vec3 bgcolor, vec3 addcolor) {
   	vec3 yuvBg = rgbToYuv(bgcolor);
    if (yuvBg.x > 0.5) {
        return colorScreen(bgcolor, addcolor);
    }
    else {
        return colorMultiply(bgcolor, addcolor);
    }    
}

//-------------------------------------------------
// Start of lava-specific stuff
//-------------------------------------------------

//Rename to getFlowSpotsBase
float getFlowSpots(vec2 uv, float sizeOfTexture) {
    //Based on https://thebookofshaders.com
    vec2 q = vec2(0.);
    q.x = fbm( uv + .5 * iTime); //Some wobbliness (fake heat)
    q.y = fbm( uv + vec2(1.0));
    
	float speed = .1;
    float wobbliness = .02;
    vec2 r = vec2(0.);
    r.x = .5 * fbm(.1* (uv + 1.0*q + vec2(5.,1.2) + wobbliness*iTime) );
    r.y = speed * iTime + 1.01 * fbm(.1 * uv + wobbliness * iTime);
	float flowFactor = fbm(sizeOfTexture * (uv + r));
    return flowFactor;
}

float getFlowSpots(vec2 uv, float sizeOfTexture, float speed) {
    //Based on https://thebookofshaders.com
    uv.y /= 1.2;
    //uv.x /= 1.5;
    vec2 q = vec2(0.);
    q.x = fbm( uv + .5 * iTime); //Some wobbliness (fake heat)
    q.y = fbm( uv + vec2(1.0));
    
	//float speed = .4;
    float wobbliness = .05;
    vec2 r = vec2(0.);
    r.x = .5 * fbm(.2* (uv + 1.0 * q + vec2(5.,1.2) + wobbliness*iTime) );
    r.y = speed * iTime + .2 * fbm(3.8 * uv + wobbliness * iTime);
	float flowFactor = fbm(1.5 * (uv + r) * .9);
    //flowFactor = clamp(flowFactor, .4, 1.7);
    return flowFactor;
}

//Something more interesting can be done here
float getLavaStoneTexture(vec2 uv, float f) {
    return getFlowSpots(uv, f);
}

vec3 getColorOfDots(vec2 uv, float f) {
    vec3 flowColor = colorBurn(middleRed1, .8 * vec3(f));
	float f2 = smoothstep(.4, 1., f);
    f2 = smoothstep(0., 1., f);
    flowColor = mix(colorDodge(orange), flowColor, f2);
    f = smoothstep(.4, 1., f);
    flowColor = mix(flowColor, flowColor * .3, f);
    flowColor = mix(flowColor, colorMultiply(flowColor, vec3(.5)), getLavaStoneTexture(uv, 94.4));
    return flowColor;
}

//Old version, looks more cloudy than current.
vec3 getColorOfBackgroundFlow2(vec2 uv, float f) {
    vec3 middleRed1 = vec3(.5882, .2451, .1922);
    vec3 middleRed2 = vec3(.8196, .3451, .3176);
    vec3 orange = vec3(0.65, 0.37, 0);
    vec3 flowColor = middleRed1;
    
    vec2 q = vec2(0.);

    vec2 r = vec2(0.);
    //r.x = .2 * fbm(uv + 1.0*q + vec2(5.7,1.2)+ 0.*iTime);
    //r.y = .2 * iTime + fbm( uv + 1.0 * q + vec2(50.3, 10.8) + 0.126*iTime);
    
    vec2 r3 = vec2(0.);
    //r3.x = .2 * fbm(uv + 1.0*q + vec2(5.7,1.2)+ 0.*iTime);
    r3.y = .2 * iTime + fbm( .3 * uv + 1.0 * q + vec2(10.3, 70.8) + .00126*iTime);

    float flowFactor = fbm(10. * (uv + r));
    float flowFactor2 = fbm(5. * (uv + r + vec2(1.)));
    float flowFactor3 = flowFactor = fbm(4.1 * (uv + r3));
    
    flowColor = mix(flowColor, colorBurn(middleRed2, vec3(.7)), flowFactor);
    flowColor = mix(flowColor, colorBurn(orange, vec3(.8)), flowFactor2);
    flowColor = mix(flowColor, colorBurn(flowColor, vec3(.8)), flowFactor2);
    flowColor = mix(orange, flowColor, smoothstep(.6, 1., f));
    flowColor = mix(colorDodge(flowColor), flowColor, smoothstep(.7, 1., f));
    flowColor = mix(colorBurn(flowColor, vec3(.75)), flowColor, flowFactor3);
    flowColor = mix(flowColor, colorDodge(flowColor), .2 * smoothstep(.6, .8, flowFactor3));

    return flowColor;
}

//Not entirely happy with the look; looks to much like clouds rather than a fluid.
vec3 getColorOfBackgroundFlow(vec2 uv, float f) {
    f = 1. - sin(f);
    vec3 flowColor = colorBurn(middleRed1, vec3(f));
	float f2 = smoothstep(.4, 1., f);
    f2 = smoothstep(0., 1., f);
    flowColor = mix(colorDodge(orange), flowColor, f2);
    f = smoothstep(.2, 1., f);
    flowColor = mix(flowColor, flowColor * .3, f);
    //flowColor = mix(flowColor, colorMultiply(flowColor, vec3(.5)), getLavaStoneTexture(uv, 10.));
    
    return  flowColor;
}

vec3 addGlowOfSpots(vec2 uv, float f, vec3 flowColor) {
    vec3 glowColor = mix(orange, flowColor, smoothstep(.7, 1., f));
    glowColor = mix(colorDodge(glowColor), flowColor, smoothstep(.9, 1., f));
    return mix(flowColor, glowColor, smoothstep(.1, .8, f));
}

float lavaEdgeAnimator(float edge, vec2 uv) {
	float edgeMovement = 0.5 * (sin(iTime/2.0 + uv.y) + 1.0) * 0.3;
    float mySin = .7 * (sin(iTime + uv.y) + 1.0);
    float mySin2 = .7 * (sin(iTime + uv.y + 2.1415) + 1.0);
    edge += mySin * edgeMovement; 
    edge += mySin2 * edgeMovement;
	return edge;
}

vec3 addGlowToEdges(vec2 uv, float f, vec3 flowColor) {
    float uvScaler = uv.x/uv.y;
    float MWF = .1 * f * getFlowSpots(uv, 30.); 
    float edges = smoothstep(0.0 + .1 * MWF, MWF * 2., uv.x) - 
        smoothstep((1. - MWF) * uvScaler, (1.0 - .1 * MWF) * uvScaler, uv.x);
    float edges2 = smoothstep(0.0 + .1 * MWF, MWF, uv.x) - 
        smoothstep((1. - MWF * .5) * uvScaler, (1.0 - .05 * MWF) * uvScaler, uv.x);
    edges = lavaEdgeAnimator(edges, uv);
    edges2 = lavaEdgeAnimator(edges2, uv);
    //flowColor = mix(orange, flowColor, clamp(edges, .0, 1.));
    flowColor = mix(orange, flowColor, clamp(edges2, .2, 1.));
    return mix(colorDodge(flowColor), flowColor, clamp(edges, .2, 1.));
}

float filterEdgesOfLava(float f, vec2 uv) {
    //We want the floating stuff to stay out of the edges.
    //There should be a better way to do this though!
    float uvScaler = uv.x/uv.y;
    float MWF = .003 + .2 * f; 
    float edges = smoothstep(0.0 + .7 * MWF, MWF, uv.x) - 
        smoothstep((1. - MWF) * uvScaler, (1.0 - .3 * MWF) * uvScaler, uv.x);
    f = mix(1.0, f, clamp(edges, .0, 1.));
    return f;
}

float filterFlowToTexture(float f, vec2 uv) {
    f += smoothstep(.3, .5, f) * 0.8 * (1. - f);
    f = smoothstep(0., 1., f);
    f = filterEdgesOfLava(f, uv);
    return f;
}

void main()                                                                                 
{
	// Normalized pixel coordinates (from 0 to 1)
    uv = TexCoord0;
	uv.y = -uv.y*10.0f;
	//uv.x = uv.x + 100.f;
	
    
	float flowSpots = getFlowSpots(uv, 4.4);
    float flowSpots2 = getFlowSpots(uv, 2.4, .13);
    float f = 1. - flowSpots;
    f = filterFlowToTexture(f, uv);
    
    vec3 color = mix(getColorOfBackgroundFlow(uv, flowSpots2), getColorOfDots(uv, flowSpots), step(f, .5));
	color = addGlowOfSpots(uv, f, color);
    color = addGlowToEdges(uv, flowSpots, color);
    FragColor = vec4(color, 1.);
}
