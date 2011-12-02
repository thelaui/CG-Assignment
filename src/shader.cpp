# include "include/shader.hpp"

# include <iostream>
# include <fstream>
# include <stdio.h>
# include <stdlib.h>
# include <cstring>

# include <GL/glew.h>
# include <GL/freeglut.h>

namespace shader {

namespace {

    static char* textFileRead(std::string const& fileName) throw (std::string) {
        char* text = 0;

        FILE *file = fopen(fileName.c_str(), "rt");

        if (file != 0) {
            fseek(file, 0, SEEK_END);
            int count = ftell(file);
            rewind(file);

            if (count > 0) {
                text = (char*)malloc(sizeof(char) * (count + 1));
                count = fread(text, sizeof(char), count, file);
                text[count] = '\0';
            }
            fclose(file);
        }
        else {
            throw std::string("File not found" + fileName);
        }
        return text;
    }

    static void validateShader(unsigned shader, const char* file = 0) {
        const unsigned int BUFFER_SIZE = 512;
        char buffer[BUFFER_SIZE];
        memset(buffer, 0, BUFFER_SIZE);
        GLsizei length = 0;

        glGetShaderInfoLog(shader, BUFFER_SIZE, &length, buffer);
        if (length > 0) {
            std::cerr << "Shader " << shader << " (" << (file?file:"") << ") compile Log: " << buffer << std::endl;
        }
    }

}

unsigned load(std::string const& fileName, GLenum shaderType) throw (std::string) {

	unsigned shader_id = 0;
	FILE* file;
	long file_size = -1;
	const char* glsl_source;

	glsl_source = textFileRead(fileName);

	shader_id = glCreateShader(shaderType);

	glShaderSource(shader_id, 1, &glsl_source, 0);
	glCompileShader(shader_id);
	validateShader(shader_id);

	return shader_id;
}


}
