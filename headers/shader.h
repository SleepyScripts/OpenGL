#ifndef SHADER_H
#define SHADER_H
#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>

const char *ReadShaderFile(const char *source);

unsigned int CreateShaderProgram(const char *vertexFile, const char *fragmentFile);

#endif
