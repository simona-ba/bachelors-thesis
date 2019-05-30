#version 330 core
out vec4 FragColor;

in vec3 Normal;  
in vec3 FragPos;  
in vec3 Tangent; 
in mat3 TBN;
  
uniform vec3 lightPos; 
uniform vec3 viewPos; 
uniform vec3 lightColor;

uniform int viewMode;

uniform int specularEnabled;
uniform int blinnEnabled;

in vec2 TexCoords;
uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;
uniform sampler2D texture_normal1;

#define DEFAULT 0
#define WIREFRAME 1
#define ALBEDO 2
#define UV 3
#define LIGHTING_ONLY 4
#define NORMALS 5
#define SPECULAR_MAP 6
#define NORMAL_MAP 7

void drawDefault();
void drawAlbedo();
void drawWireframe();
void drawUV();
void drawLightingOnly();
void drawSpecularMap();
void drawNormalMap();
vec4 CalcLighting(vec3 objectColor);

void main()
{
	if(viewMode == DEFAULT)
	{
		drawDefault();
	}
	else if(viewMode == WIREFRAME)
	{
		drawWireframe();
	}
	else if(viewMode == ALBEDO || viewMode == NORMALS)
	{
		drawAlbedo();
	}
	else if(viewMode == UV)
	{
		drawUV();
	}
	else if(viewMode == LIGHTING_ONLY)
	{
		drawLightingOnly();
	}
	else if(viewMode == SPECULAR_MAP)
	{
		drawSpecularMap();
	}
	else if(viewMode == NORMAL_MAP)
	{
		drawNormalMap();
	}
} 

void drawDefault()
{
	vec3 albedo = texture(texture_diffuse1, TexCoords).rgb;
	FragColor = CalcLighting(albedo);
}

void drawAlbedo()
{
	FragColor = texture(texture_diffuse1, TexCoords);
}

void drawWireframe()
{
	FragColor = vec4(1.f, 0.f, 1.f, 1.f);
}

void drawUV()
{
	FragColor = vec4(mod(TexCoords.r, 1), mod(TexCoords.g, 1), 0.f, 1.f);
}

void drawLightingOnly()
{
	FragColor = CalcLighting(vec3(1.f, 1.f, 1.f));
}

void drawSpecularMap()
{
	FragColor = texture(texture_specular1, TexCoords);
}

void drawNormalMap()
{
	FragColor = texture(texture_normal1, TexCoords);
}

vec4 CalcLighting(vec3 objectColor)
{
	//vec3 norm = texture(texture_normal1, TexCoords).rgb;
	//norm = normalize(norm * 2.0 - 1.0);   
	//norm = normalize(TBN * norm); 

	vec3 norm = normalize(Normal);

	// ambient
    float ambientStrength = 0.4;
    vec3 ambient = ambientStrength * lightColor;  

	// diffuse 
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;
    
    // specular	
	vec3 specular = vec3(0.f);

	if(specularEnabled == 1)
	{
		vec3 lightDir = normalize(lightPos - FragPos);
		vec3 viewDir = normalize(viewPos - FragPos);
		vec3 specularBase = texture(texture_specular1, TexCoords).rgb;
		float specularStrength = 1.5;
		float spec = 0.f;

		if(blinnEnabled == 1)
		{		
			vec3 halfwayDir = normalize(lightDir + viewDir);
			spec = pow(max(dot(norm, halfwayDir), 0.0), 32.f);
		}
		else
		{
			vec3 reflectDir = reflect(-lightDir, norm);
			spec = pow(max(dot(viewDir, reflectDir), 0.0), 8.f);
		}
		
		specular = specularBase * specularStrength * spec * lightColor; 
	}
        
    vec3 result = (ambient + diffuse + specular) * objectColor;
    return vec4(result, 1.0);
}