#version 460 core

out vec4 fragColor;
uniform vec3 viewPos;
uniform sampler2D u_Texture;
uniform mat3 u_InverseModel;

in vec2 v_TexCoord;
varying vec3 v_Normal;
varying vec3 v_fragPos;

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
}; 
  
#define POINT_LIGHT_COUNT |LIGHT_COUNT|
struct point_Light {
    int activated;
    vec3 position;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

#define DIRECTIONAL_LIGHT_COUNT |DIR_LIGHT_COUNT|
struct directional_Light {
    int activated;
    vec3 position;
    vec3 lightDir;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform point_Light p_light[POINT_LIGHT_COUNT];  

uniform directional_Light d_light[DIRECTIONAL_LIGHT_COUNT];  

uniform Material material;

vec3 CalcPointLight(point_Light light, vec3 normal, vec3 fragPos, vec3 viewPos)
{
    vec3 uNormal = u_InverseModel * normalize(normal);
    vec3 lightDir = normalize(light.position - fragPos);

    vec3 ambient = material.ambient * light.ambient;

    float diff = max(dot(uNormal, lightDir), 0);
    vec3 diffuse = light.diffuse * (diff * material.diffuse);


    vec3 viewDir = normalize(viewPos - v_fragPos);
    vec3 halfwayDir = normalize(lightDir + viewDir);

    float spec = pow(max(dot(uNormal, halfwayDir), 0.0), material.shininess);
    vec3 specular =  light.specular * (spec * material.specular);  

    float distance    = length(light.position - v_fragPos);
    //float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));   
    float attenuation = 1/(distance * distance); 

    ambient*=attenuation;
    diffuse*=attenuation;
    specular*=attenuation;

    return ambient + diffuse + specular;
}

void main()
{
    vec4 texColor = texture(u_Texture, v_TexCoord);
    vec3 lightColor = vec3(1,1,1);
    vec3 ambientColor = vec3(1,1,1);


    vec3 lightingResult = vec3(0,0,0);

    for (int i=0;i<POINT_LIGHT_COUNT;i++)
    {
        if (p_light[i].activated==1)
        {
            lightingResult+=CalcPointLight(p_light[i], v_Normal, v_fragPos, viewPos);
        }
    }

    fragColor = vec4(lightingResult, 1) * texColor;
    
    //fragColor = texColor;
    //fragColor = vec4(v_normal, 1);
}