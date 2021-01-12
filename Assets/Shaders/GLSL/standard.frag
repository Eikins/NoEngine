#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(push_constant) uniform ConstantBuffer 
{ 
    mat4 M_View;
    mat4 M_Proj;
    vec3 W_CameraPos;
    vec3 W_LightDir;
    vec3 W_LightColor;
} constantBuffer;

layout(location = 0) in struct
{
    vec3 wNormal;
    vec3 wTangent;
    vec2 texCoords;
} In;

layout(location = 0) out vec4 fragColor;

void main() {
    vec3 normal = normalize(In.wNormal);
    vec3 color = vec3(0.8, 0.2, 0.2);
    vec3 direct = max(0, dot(-constantBuffer.W_LightDir, normal)) * color * constantBuffer.W_LightColor;

    fragColor = vec4(direct, 1.0);
}

