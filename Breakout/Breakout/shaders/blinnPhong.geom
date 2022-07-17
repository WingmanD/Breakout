#version 460 core
layout(triangles) in;
layout(triangle_strip, max_vertices = 150) out;

in vertexData {
    vec3 position;
    vec3 normal;
    vec2 texCoord;
} vertices[];

out fragmentData {
    vec3 position;
    vec3 normal;
    vec2 texCoord;
} fragment;

layout (std140, binding = 0) uniform ViewData {
    mat4 view;
    mat4 projection;

    vec4 cameraPos;
    vec4 unlit;
};

void main() {
    vec3 normal = normalize(cross(gl_in[1].gl_Position.xyz - gl_in[0].gl_Position.xyz, gl_in[2].gl_Position.xyz - gl_in[0].gl_Position.xyz));

    vec3 triangleCenter = (gl_in[0].gl_Position + gl_in[1].gl_Position + gl_in[2].gl_Position).xyz / 3.0;
    vec3 cameraDir = normalize(cameraPos.xyz - triangleCenter);

    /*if (dot(normal, cameraDir) < 0.0) {
        EndPrimitive();
        return;
    }*/

    for (int i = 0; i < 3; i++) {
        gl_Position = projection * view * gl_in[i].gl_Position;
        fragment.position = gl_in[i].gl_Position.xyz;
        fragment.normal = vertices[i].normal;
        fragment.texCoord = vertices[i].texCoord;

        EmitVertex();
    }

    EndPrimitive();
}