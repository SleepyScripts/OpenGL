#version 330 core
out vec4 FragColor;
in vec3 normal;
in vec2 uv;

void main() {
    FragColor = vec4(normal.xyz, 1.0);
} 
