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
#include "skeleton.h"
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
			std::map<string, int> bones;
			bool anim = false;
			//*bone data
			glm::mat4 globalInverseTransform;
			glm::mat4 boneTransforms[MAX_BONES];
			int numBones = 0;
			
													/**  Functions   */
			/**Constructor*/
			Model(GLchar* path, bool gamma = false);

			void Draw(Shader shader);
			void UpdateSkeleton();

		private:
			const aiScene* scene;
			std::vector<aiNode*> ai_nodes;
			std::vector<aiNodeAnim*> ai_nodes_anim;

													/*  Functions   */
			void loadModel(string const &path);
			void processNode(aiNode* node, const aiScene* scene);
			void processAnim(const aiScene* scene);
			Mesh processMesh(aiMesh* mesh, const aiScene* scene);
			std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type,
				std::string typeName);
			/** LoadBones and bone data into vertices*/
			void Model::loadBones(unsigned int MeshIndex, const aiMesh* pMesh);
			/** Animation time for bone transforms*/
			std::vector<glm::mat4> BoneTransform(aiScene* scene, float TimeInSeconds, std::vector<glm::mat4>& Transforms);
			void ReadNodeHeirarchy(float AnimationTime, aiScene* scene, aiNode* pNode, const glm::mat4 ParentTransform);   /** read and get bone transform from node hierarchy*/
			
			void CalcInterpolatedScale(aiVector3D Out, float AnimationTime, const aiNodeAnim * pNodeAnim);
			void CalcInterpolatedPosition(aiVector3D Out, float AnimationTime, const aiNodeAnim * pNodeAnim);
			void CalcInterpolatedRotation(aiQuaternion& Out, float AnimationTime, const aiNodeAnim * pNodeAnim);
			
			unsigned int FindScale(float AnimationTime, const aiNodeAnim* pNodeAnim);
			unsigned int FindPosition(float AnimationTime, const aiNodeAnim* pNodeAnim);
			unsigned int FindRotation(float AnimationTime, const aiNodeAnim* pNodeAnim);
			Bone* FindBone(std::string name); // Find bone in bones list
			aiNode* FindAiNode(std::string name); // find aiNode in model
			aiNodeAnim* FindAiNodeAnim(std::string name); // find animation node in model


			
		};
	}
}

#endif