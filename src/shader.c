#include <shader.h>

const char *ReadShaderFile(const char *source){
    FILE *file = fopen(source, "rb");
    if(!file) {
		fprintf(stderr,"\033[1;31m%s\033[0m:%s:%s:%d\n", "ERROR", __FILE__, __func__,  __LINE__);
		exit(-1);
	}
    fseek(file, 0, SEEK_END);
    size_t size = ftell(file);
    rewind(file);
    char *string = (char*)malloc(sizeof(char)*(size+1));
    size_t ret = fread(string, 1, size, file);
    string[size] = '\0';
    if(size != ret) {
    	fclose(file);
		fprintf(stderr,"\033[1;31m%s\033[0m:%s:%s:%d\n", "ERROR", __FILE__, __func__,  __LINE__);
		exit(-1);
	}
    fclose(file);
    return string;
}

unsigned int CreateShaderProgram(const char *vertexFile, const char *fragmentFile) {
	int ret;
	char log[512];
	
	const char *vertexSource   = ReadShaderFile(vertexFile);
	const char *fragmentSource = ReadShaderFile(fragmentFile);

	unsigned int vertexShader  = glCreateShader(GL_VERTEX_SHADER);

	glShaderSource(vertexShader, 1, &vertexSource, NULL);
	glCompileShader(vertexShader);
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &ret);
	if (!ret) {
		glGetShaderInfoLog(vertexShader, 512, NULL, log);
		fprintf(stderr, "\033[1;35m%s\033[0m%s:%s:%d:%s\n", "ERROR", __FILE__, __func__, __LINE__, log);
	}

	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &ret);
	if (!ret) {
		glGetShaderInfoLog(fragmentShader, 512, NULL, log);
		fprintf(stderr, "\033[1;35m%s\033[0m%s:%s:%d:%s\n", "ERROR", __FILE__, __func__, __LINE__, log);
	}

	unsigned int shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	glGetShaderiv(shaderProgram, GL_LINK_STATUS, &ret);
	if (!ret) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, log);
		fprintf(stderr, "\033[1;35m%s\033[0m%s:%s:%d:%s\n", "ERROR", __FILE__, __func__, __LINE__, log);
	}
	
	return shaderProgram;
}
