#version 430
in vec2 TexCoord0;
in vec3 normals;
in vec3 vertPos;

uniform sampler2D textu;

uniform vec3 in_lightPos;
uniform vec3 in_color;
uniform vec3 planeHit;


const vec3 lightPos = in_lightPos;
const vec3 ambientColor = in_color;
const vec3 diffuseColor = in_color;
const vec3 specColor = vec3(1.0, 1.0, 1.0);
const float shininess = 16.0;
const float screenGamma = 1.2; // Assume the monitor is calibrated to the sRGB color space


out vec4 Color;

void main()
{
	
	vec3 normal = normalize(normals);
	vec3 lightDir = normalize(lightPos - vertPos);
	
	float lambertian = max(dot(lightDir,normal), 0.0);
	float specular = 0.0;
	if(lambertian > 0.0) 
	{
		vec3 viewDir = normalize(-vertPos);
		
		// this is blinn phong
		vec3 halfDir = normalize(lightDir + viewDir);
		float specAngle = max(dot(halfDir, normal), 0.0);
		specular = pow(specAngle, shininess);
	}
	vec3 colorLinear = ambientColor + lambertian * diffuseColor + specular * specColor;
	vec3 colorGammaCorrected = pow(colorLinear, vec3(1.0/screenGamma));
	
	//Color = texture(textu, TexCoord0) * vec4(colorGammaCorrected, 1.0) * vec4(planeHit, 1.0);
	
	Color = texture(textu, TexCoord0);
}
