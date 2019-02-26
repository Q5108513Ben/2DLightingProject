#pragma once

#include <glad/glad.h>

#include <iostream>
#include <sstream>
#include <fstream>

namespace setup {

	static std::string loadFile(std::string file_location) {

		std::ifstream input_file(file_location);
		std::stringstream string_stream;

		string_stream << input_file.rdbuf();

		return string_stream.str();

	}

	static GLuint shader(GLuint shader_type, std::string file_location) {

		GLuint shader = glCreateShader(shader_type);

		std::string sshader_source = setup::loadFile(file_location);
		const char* cshader_source = sshader_source.c_str();

		glShaderSource(shader, 1, (const GLchar**)&cshader_source, NULL);
		glCompileShader(shader);

		GLint compile_status{ GL_FALSE };
		glGetShaderiv(shader, GL_COMPILE_STATUS, &compile_status);

		if (compile_status != GL_TRUE) {

			const int string_length{ 1024 };
			GLchar log[string_length]{ "" };

			glGetShaderInfoLog(shader, string_length, NULL, log);

			std::cerr << log << std::endl;

		}

		return shader;

	}

	static void checkLink(GLuint shader) {

		GLint link_status{ GL_FALSE };
		glGetProgramiv(shader, GL_LINK_STATUS, &link_status);

		if (link_status != GL_TRUE) {

			const int string_length{ 1024 };
			GLchar log[string_length]{ "" };

			glGetProgramInfoLog(shader, string_length, NULL, log);

			std::cerr << log << std::endl;

		}

	}

}