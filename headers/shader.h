#ifndef SHADER_H
#define SHADER_H
#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>

const char *ReadShaderFile(const char *source);

unsigned int ShaderCreate(const char *vertexFile, const char *fragmentFile);

void ShaderSetColor(unsigned int shader, float r, float g, float b, float alpha);

#endif
