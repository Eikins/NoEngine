#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(binding = 0) uniform PerObjectBuffer {
    mat4 M_LocalToWorld;
} perObjectBuffer;

layout(push_constant) uniform ConstantBuffer 
{ 
    mat4 M_View;
    mat4 M_Proj;
    vec3 W_CameraPos;
} constantBuffer;

layout(location = 0) in vec3 InPosition;
layout(location = 1) in vec3 InNormal;
layout(location = 2) in vec4 InTangent;
layout(location = 3) in vec2 InTexCoords;

layout(location = 0) out struct
{
    vec3 wNormal;
    vec3 wTangent;
    vec2 texCoords;
} Out;

void main() {
    mat4 localToClip = constantBuffer.M_Proj * constantBuffer.M_View  * perObjectBuffer.M_LocalToWorld;

    gl_Position = localToClip * vec4(InPosition, 1.0);
    Out.wNormal = (localToClip * vec4(InNormal, 0.0)).xyz;
    Out.wTangent = (localToClip * vec4(InTangent.xyz, 0.0)).xyz;
    Out.texCoords = InTexCoords;
}

