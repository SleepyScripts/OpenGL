#version 330 core
out vec4 FragColor;
in vec2 outUvs;

uniform vec4 color;
void main() {
    FragColor = vec4(outUvs.x, outUvs.y, 0.0, 1.0);
} 
