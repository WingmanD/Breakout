#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aCol;
layout (location = 2) in vec3 aNormal;
layout (location = 3) in vec2 aUV;

out vertexData {
    vec3 position;
    vec3 normal;
    vec2 texCoord;
} vertex;

uniform mat4 model;

uniform vec2 textureScale;
uniform vec2 textureOffset;

void main()
{
    vec4 worldSpace =  model * vec4(aPos, 1.0);
    vertex.position = (worldSpace/worldSpace.w).xyz;

    mat3 normalMatrix = mat3(transpose(inverse(model)));
    vertex.normal = normalize(normalMatrix * aNormal);
    gl_Position =  worldSpace;

    vertex.texCoord = aUV * textureScale + textureOffset;
}
