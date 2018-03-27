#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <GL/glew.h>
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"


namespace sa3d {
	namespace graphics {


		class Shader
		{
		private:
			GLuint m_ShaderID;
			const char *m_VertPath;
			const char *m_FragPath;
		public:
			GLuint program;
			Shader(const char* vertPath, const char* fragPath);
			~Shader();

			/**Functions set values for shaders in file*/
			void setUniform1f(const GLchar* name, float value);
			void setUniform1i(const GLchar* name, int value);
			void setUniform2f(const GLchar* name, const glm::vec2& vector);
			void setUniform3f(const GLchar* name, const glm::vec3& vector);
			void setUniform4f(const GLchar* name, const glm::vec4& vector);
			void setUniformMat4(const GLchar* name, const glm::mat4& matrix);


			/** Enable/Disable shader*/
			void enable() const;
			void disable() const;
		private:
			/**Gets values for shaders in function*/
			GLint getUniformLocation(const GLchar* name);
			GLuint load();
			void checkCompileErrors(unsigned int shader, std::string type);

		};




	}
}