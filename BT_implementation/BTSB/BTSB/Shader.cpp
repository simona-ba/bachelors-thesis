#include "Shader.h"
#include <glad/glad.h>

Shader::Shader(const std::string vertex_path, const std::string fragment_path, const std::string geometry_path)
{
	// 1. retrieve the vertex/fragment source code from filePath
	std::string vertex_code;
	std::string fragment_code;
	std::string geometry_code;
	std::ifstream v_shader_file;
	std::ifstream f_shader_file;
	std::ifstream g_shader_file;
	// ensure ifstream objects can throw exceptions:
	v_shader_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	f_shader_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	g_shader_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try
	{
		// open files
		v_shader_file.open(vertex_path);
		f_shader_file.open(fragment_path);

		std::stringstream v_shader_stream, f_shader_stream;
		// read file's buffer contents into streams
		v_shader_stream << v_shader_file.rdbuf();
		f_shader_stream << f_shader_file.rdbuf();

		// close file handlers
		v_shader_file.close();
		f_shader_file.close();
		// convert stream into string
		vertex_code = v_shader_stream.str();
		fragment_code = f_shader_stream.str();

		if (geometry_path != "")
		{
			g_shader_file.open(geometry_path);
			std::stringstream gShaderStream;
			gShaderStream << g_shader_file.rdbuf();
			g_shader_file.close();
			geometry_code = gShaderStream.str();
		}
	}
	catch (std::ifstream::failure e)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
	}
	const char* v_shader_code = vertex_code.c_str();
	const char* f_shader_code = fragment_code.c_str();
	const char* g_shader_code = geometry_code.c_str();

	// 2. compile shaders
	unsigned int vertex, fragment, geometry;
	int success;
	char info_log[512];

	// vertex Shader
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &v_shader_code, NULL);
	glCompileShader(vertex);
	// print compile errors if any
	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertex, 512, NULL, info_log);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << info_log << std::endl;
	}

	// similar for Fragment Shader
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &f_shader_code, NULL);
	glCompileShader(fragment);

	glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);

	if (!success) {
		glGetShaderInfoLog(fragment, 512, NULL, info_log);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << info_log << std::endl;
	}

	if (geometry_path != "")
	{
		// similar for Geomtery Shader
		geometry = glCreateShader(GL_GEOMETRY_SHADER);
		glShaderSource(geometry, 1, &g_shader_code, NULL);
		glCompileShader(geometry);

		glGetShaderiv(geometry, GL_COMPILE_STATUS, &success);

		if (!success) {
			glGetShaderInfoLog(geometry, 512, NULL, info_log);
			std::cout << "ERROR::SHADER::GEOMETRY::COMPILATION_FAILED\n" << info_log << std::endl;
		}
	}

	// shader Program
	ID = glCreateProgram();
	glAttachShader(ID, vertex);
	glAttachShader(ID, fragment);

	if (geometry_path != "")
	{
		glAttachShader(ID, geometry);
	}

	glLinkProgram(ID);
	// print linking errors if any
	glGetProgramiv(ID, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(ID, 512, NULL, info_log);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << info_log << std::endl;
	}

	// delete the shaders as they're linked into our program now and no longer necessery
	glDeleteShader(vertex);
	glDeleteShader(fragment);

	if (geometry_path != "")
	{
		glDeleteShader(geometry);
	}
}

void Shader::use()
{
	glUseProgram(ID);
}

void Shader::setBool(const std::string &name, bool value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}
void Shader::setInt(const std::string &name, int value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}
void Shader::setFloat(const std::string &name, float value) const
{
	glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setMat4(const std::string &name, const glm::mat4 &mat) const
{
	glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void Shader::setVec3(const std::string &name, const glm::vec3 &value) const
{
	glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
}

void Shader::setVec3(const std::string &name, float x, float y, float z) const
{
	glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z);
}

