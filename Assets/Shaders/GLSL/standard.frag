#version 450
#extension GL_ARB_separate_shader_objects : enable

// layout(set = 0, binding = 0) uniform sampler2D _SamplerAlbedo;

layout(location = 0) in struct
{
    vec3 wNormal;
    vec3 wTangent;
    vec2 texCoords;
} In;

layout(location = 0) out vec4 fragColor;

void main() {
    vec3 lightDir = normalize(vec3(1.0, 1.0, 1.0));
    vec3 color = vec3(0.8, 0.2, 0.2);
    vec3 direct = max(0, dot(lightDir, In.wNormal)) * color;

    fragColor = vec4(direct, 1.0);
}

