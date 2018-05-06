#pragma once
#ifndef MODEL_H
#define MODEL_H

#include <glad/glad.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <map>

#include "shader.h"
#include "graphics.h"
#include "mesh.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "stb_image.h"
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
using namespace std;

namespace sa3d {
	namespace graphics {

		unsigned int TextureFromFile(const char *path, const string &directory, bool gamma = false);

		class Model
		{
		public:
			/**  Model Data  */
			std::vector<Mesh> meshes;
			std::string directory;
			std::vector<Texture> textures_loaded;	// Stores all the textures loaded so far, optimization to make sure textures aren't loaded more than once.
			bool gammaCorrection;
													/**  Functions   */
			/**Constructor*/
			Model(GLchar* path, bool gamma = false);

			void Draw(Shader shader);
			

													/*  Functions   */
		private:
			void loadModel(string const &path);
			void processNode(aiNode* node, const aiScene* scene);
			Mesh processMesh(aiMesh* mesh, const aiScene* scene);
			std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type,
				std::string typeName);

		};
	}
}

#endif