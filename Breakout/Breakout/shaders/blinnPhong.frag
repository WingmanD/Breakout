#version 460 core
out vec4 FragColor;

in fragmentData {
    vec3 position;
    vec3 normal;
    vec2 texCoord;
} fragment;


struct Light {
    vec4 position;
    vec4 color;
    float intensity;
};

layout(std430, binding = 2) readonly buffer LightData {
    Light lights[];
};

uniform vec3 ambientColor;
uniform vec3 diffuseColor;
uniform vec3 specularColor;
uniform float shininess;
uniform float opacity;

layout (std140, binding = 0) uniform ViewData {
    mat4 view;
    mat4 projection;

    vec4 cameraPos;
    vec4 unlit;
};

uniform int diffuseMapPresent;
uniform sampler2D diffuseMap;

uniform int specularMapPresent;
uniform sampler2D specularMap;

void main()
{
    vec3 n = fragment.normal;
    vec3 postion = fragment.position;

    vec3 v = normalize(cameraPos.xyz - postion);

    vec3 ambient = ambientColor * 0.05;

    vec3 diffuse = vec3(0);
    vec3 specular = vec3(0);

    for (uint i = 0; i < lights.length(); i++) {
        Light light = lights[i];
        vec3 l_orig = light.position.xyz - postion;


        const float kc = 1.0;
        const float kl = 0.09;
        const float kq = 0.032;

        float dl = length(l_orig);

        float denominator = kc + kl * dl + kq * dl * dl;

        vec3 l = normalize(l_orig);
        vec3 r = reflect(-l, n);

        vec3 h = normalize(l + v);

        diffuse += light.intensity * light.color.xyz * max(dot(l, n), 0) / denominator;
        specular += light.intensity * light.color.xyz * pow(max(dot(h, n), 0), shininess) / denominator;
    }

    if (diffuseMapPresent == 1) diffuse *= texture(diffuseMap, fragment.texCoord).xyz;
    else diffuse *= diffuseColor;

    if (specularMapPresent == 1) specular *= texture(specularMap, fragment.texCoord).xyz;
    else specular *= specularColor;

    if (unlit.x == 1) {
        if (diffuseMapPresent == 1) diffuse = texture(diffuseMap, fragment.texCoord).xyz;
        else diffuse = diffuseColor;
    }
    
    vec3 color = ambient + diffuse + specular;

    FragColor = vec4(color, opacity);
} 