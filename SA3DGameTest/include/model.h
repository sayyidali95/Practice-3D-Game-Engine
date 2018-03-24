#pragma once
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <GL/glew.h>

#include "shader.h"
#include "graphics.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "stb_image.h"
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"


namespace sa3d {
	namespace graphics {

		GLint TextureFromFile(const char* path, std::string directory);

		class Model
		{
		public:
			/**  Functions   */
			/**Constructor*/
			Model(GLchar* path);

			void Draw(Shader shader);
		private:
			/**  Model Data  */
			std::vector<Mesh> meshes;
			std::string directory;
			std::vector<Texture> textures_loaded;	// Stores all the textures loaded so far, optimization to make sure textures aren't loaded more than once.

													/*  Functions   */
			void loadModel(std::string path);
			void processNode(aiNode* node, const aiScene* scene);
			Mesh processMesh(aiMesh* mesh, const aiScene* scene);
			std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type,
				std::string typeName);

		};
	}
}