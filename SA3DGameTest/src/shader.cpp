#include "shader.h"

namespace sa3d {
	namespace graphics {

		Shader::Shader(const char* vertPath, const char* fragPath)
			: m_VertPath(vertPath), m_FragPath(fragPath)
		{
			m_ShaderID = load();
		}

		Shader::~Shader()
		{
			glDeleteProgram(m_ShaderID);
		}

		GLuint Shader::load()
		{
			
			GLuint vertex = glCreateShader(GL_VERTEX_SHADER);
			GLuint fragment = glCreateShader(GL_FRAGMENT_SHADER);
			std::ifstream vShaderFile;
			std::ifstream fShaderFile;
			std::string vertSourceString;
			std::string fragSourceString;


			// ensures ifstream objects can throw exceptions:
			vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
			fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
			try
			{
				/**Opens Files*/
				vShaderFile.open(m_VertPath);
				fShaderFile.open(m_FragPath);
				std::stringstream vShaderStream, fShaderStream;
				/* Read file's buffer contents into streams*/
				vShaderStream << vShaderFile.rdbuf();
				fShaderStream << fShaderFile.rdbuf();
				// close file handlers
				vShaderFile.close();
				fShaderFile.close();

				vertSourceString = vShaderStream.str();
				fragSourceString = fShaderStream.str();
			}
			catch (std::ifstream::failure e)
			{
				std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
			}

			const GLchar* vertSource = vertSourceString.c_str();
			const GLchar* fragSource = fragSourceString.c_str();



			glShaderSource(vertex, 1, &vertSource, NULL);
			glCompileShader(vertex);
			checkCompileErrors(vertex, "VERTEX");
			
			GLint result;
			glGetShaderiv(vertex, GL_COMPILE_STATUS, &result);
			if (result == GL_FALSE)
			{
				GLint length;
				glGetShaderiv(vertex, GL_INFO_LOG_LENGTH, &length);
				std::vector<char> error(length);
				glGetShaderInfoLog(vertex, length, &length, &error[0]);
				std::cout << "Failed to compile vertex shader!" << std::endl <<
					&error[0] << std::endl;
				glDeleteShader(vertex);
				return 0;
			}


			glShaderSource(fragment, 1, &fragSource, NULL);
			glCompileShader(fragment);
			checkCompileErrors(fragment, "FRAGMENT");

			glGetShaderiv(fragment, GL_COMPILE_STATUS, &result);
			if (result == GL_FALSE)
			{
				GLint length;
				glGetShaderiv(fragment, GL_INFO_LOG_LENGTH, &length);
				std::vector<char> error(length);
				glGetShaderInfoLog(fragment, length, &length, &error[0]);
				std::cout << "Failed to compile fragment shader!" << std::endl <<
					&error[0] << std::endl;
				glDeleteShader(fragment);
				return 0;
			}
			//shader program
			program = glCreateProgram();
			glAttachShader(program, vertex);
			glAttachShader(program, fragment);

			glLinkProgram(program);
			//glValidateProgram(program);

			//glDetachShader(program, vertex);
			//glDetachShader(program, fragment);

			glDeleteShader(vertex);
			glDeleteShader(fragment);

			//return program;
		}

		GLint Shader::getUniformLocation(const GLchar* name)
		{
			return glGetUniformLocation(m_ShaderID, name);
		}

		void Shader::setUniform1f(const GLchar* name, float value)
		{
			glUniform1f(getUniformLocation(name), value);
		}

		void Shader::setUniform1i(const GLchar* name, int value)
		{
			glUniform1i(getUniformLocation(name), value);
		}

		void Shader::setUniform2f(const GLchar* name, const glm::vec2& vector)
		{
			glUniform2f(getUniformLocation(name), vector.x, vector.y);
		}

		void Shader::setUniform3f(const GLchar* name, const glm::vec3& vector)
		{
			glUniform3f(getUniformLocation(name), vector.x, vector.y, vector.z);

		}

		void Shader::setUniform4f(const GLchar* name, const glm::vec4& vector)
		{
			glUniform4f(getUniformLocation(name), vector.x, vector.y, vector.z, vector.w);
		}

		void Shader::setUniformMat4(const GLchar* name, const glm::mat4& matrix)
		{
			float elements[16] = { 0.0 };
			const float *pSource = (const float*)glm::value_ptr(matrix);
			for (int i = 0; i < 16; ++i)
				elements[i] = pSource[i];

			glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, elements);
		}

		void Shader::enable() const
		{
			glUseProgram(m_ShaderID);
		}

		void Shader::disable() const
		{
			glUseProgram(0);
		}

		void Shader::checkCompileErrors(unsigned int shader, std::string type)
		{
			int success;
			char infoLog[1024];
			if (type != "PROGRAM")
			{
				glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
				if (!success)
				{
					glGetShaderInfoLog(shader, 1024, NULL, infoLog);
					std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
				}
			}
		}

	}
}