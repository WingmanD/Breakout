#version 460 core
out vec4 FragColor;

in fragmentData {
    vec3 position;
    vec3 normal;
    vec2 texCoord;
} fragment;


struct Light {
    vec3 position;
    vec3 color;
    float intensity;
};

layout(std430, binding = 0) buffer LightsBuffer {
    uint lights_length;
    Light lights[];
} lights;

uniform vec3 ambientColor;
uniform vec3 diffuseColor;
uniform vec3 specularColor;
uniform float shininess;
uniform float opacity;

/*uniform vec3 lightPosition;
uniform vec3 lightColor;
uniform float lightIntensity;*/

uniform vec3 cameraPos;

uniform int useDiffuseMap;
uniform sampler2D diffuseMap;

uniform int useSpecularMap;
uniform sampler2D specularMap;

void main()
{
    vec3 n = fragment.normal;
    vec3 postion = fragment.position;

    vec3 l = normalize(lightPosition - postion);
    vec3 v = normalize(cameraPos - postion);
    vec3 r = reflect(-l, n);

    vec3 ambient = ambientColor * 0.05;

    vec3 diffuse = vec3(0);
    if (useDiffuseMap == 1) diffuse = lightIntensity * lightColor * texture(diffuseMap, fragment.texCoord).xyz * max(dot(l, n), 0);
    else diffuse = lightIntensity * lightColor * diffuseColor * max(dot(l, n), 0);

    vec3 specular = vec3(0);
    if (useSpecularMap == 1) specular = lightIntensity * lightColor * texture(specularMap, fragment.texCoord).xyz * pow(max(dot(r, v), 0), shininess);
    else specular = lightIntensity * lightColor * specularColor * pow(max(dot(r, v), 0), shininess);

    vec3 color = ambient + diffuse + specular;

    FragColor = vec4(color, 1);
} 