#pragma once
#include "buffer.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "shader.h"

#define MAX_SHAPES 2056

namespace sa3d {
	namespace graphics {
		/**
		@brief Class for creating a shape in world space in World Space
		**/
		class Shape {
			
			glm::vec3 position;    // position of the shape
			glm::vec2 scale;		   // size of the shape
			glm::vec4 color;	   // color of the shape
			glm::vec3 rotation;

			virtual void draw(Shader shaderID);
			virtual void update();
		};


		/**
		@brief Class for creating a Cube in world space
		**/
		class Cube : public Shape {
		public:
			Cube();
		};

	}
}