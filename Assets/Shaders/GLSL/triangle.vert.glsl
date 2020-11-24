#version 450

in vec4 position;
out vec3 color;

void main() {
	color = position.xyz;
    gl_Position = position;
}