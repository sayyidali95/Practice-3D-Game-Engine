#include "model.h"

namespace sa3d {
	namespace graphics {

		Model::Model(GLchar* path, bool gamma) : gammaCorrection(gamma)
		{
			this->loadModel(path);
		}

		// Draws the model, and thus all its meshes
		void Model::Draw(Shader shader)
		{
			if (anim)
			{
				UpdateSkeleton();
			}

			for (GLuint i = 0; i < this->meshes.size(); i++)
				this->meshes[i].Draw(shader);
		}

		void Model::loadModel(string const &path)
		{
			// read file via ASSIMP
			Assimp::Importer importer;
			const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace | aiProcess_JoinIdenticalVertices);
			// check for errors
			if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
			{
				cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << endl;
				return;
			}
			// retrieve the directory path of the filepath
			directory = path.substr(0, path.find_last_of('/'));
			glBindVertexArray(0);
			//getAnimations
			processAnim(scene);

			//get global transform for skeleton
			aiMatrix4x4 GlobalInverseTransform = scene->mRootNode->mTransformation;
			GlobalInverseTransform.Inverse();

			globalInverseTransform = AiToGLMMat4(GlobalInverseTransform);

			// process ASSIMP's root node recursively
			processNode(scene->mRootNode, scene);





		}

		void Model::loadBones(unsigned int MeshIndex, const aiMesh* pMesh, vector<int>& Bones)
		{
			//load bones into bone map in mesh class
			numBones = 0;
			for (unsigned int i = 0; i < pMesh->mNumBones; i++)
			{
				unsigned int BoneIndex = 0;
				string BoneName(pMesh->mBones[i]->mName.data);

				//add bone if not in map
				if (bones.find(BoneName) == bones.end()) {
					BoneIndex = numBones;
					numBones++;

					this->bones.insert({ BoneName, BoneIndex });
				}
				else {
					BoneIndex = bones[BoneName];
				}
				//

				bones[BoneName] = BoneIndex;
				aiMatrix4x4 BoneOffset = pMesh->mBones[i]->mOffsetMatrix;
				boneTransforms[BoneIndex] = AiToGLMMat4(BoneOffset);
				//check if vertex is affected by bone weights
				for (unsigned int j = 0; j < pMesh->mBones[i]->mNumWeights; j++) {
					unsigned int VertexID = pMesh->mBones[i]->mWeights[j].mVertexId;
					float Weight = pMesh->mBones[i]->mWeights[j].mWeight;

					//add boneID and Weight
					meshes[i].vertices[VertexID].Weights[j] = Weight;
					for (unsigned int k; k < NUM_BONES_PER_VERTEX; k++)
					{
						if (meshes[i].vertices[VertexID].boneIDs[k] != NULL || BoneIndex == 0)
						{
							meshes[i].vertices[VertexID].boneIDs[k] = BoneIndex;
							break;
						}
					}
				}
			}
		}

		// Processes a node in a recursive fashion. Processes each individual mesh located at the node and repeats this process on its children nodes (if any).
		void Model::processNode(aiNode* node, const aiScene* scene)
		{


			// process each mesh located at the current node
			for (unsigned int i = 0; i < node->mNumMeshes; i++)
			{
				// the node object only contains indices to index the actual objects in the scene. 
				// the scene contains all the data, node is just to keep stuff organized (like relations between nodes).
				aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
				meshes.push_back(processMesh(mesh, scene));
			}
			// after we've processed all of the meshes (if any) we then recursively process each of the children nodes
			for (unsigned int i = 0; i < node->mNumChildren; i++)
			{
				processNode(node->mChildren[i], scene);
			}

		}

		void Model::processAnim(const aiScene* scene)
		{
			if (scene->mAnimations == 0)
				return;
			for (int i = 0; i < scene->mAnimations[0]->mNumChannels; i++)
				ai_nodes_anim.push_back(scene->mAnimations[0]->mChannels[i]);

			//We only get data from the first mAnimation because 
			//Assimp crushes all of the animation data into one
			//large sequence of data known as mAnimation.
			//Assimp does not support multiple mAnimations, surprisingly.

		}

		Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene)
		{
			// data to fill
			vector<Vertex> vertices;
			vector<unsigned int> indices;
			vector<Texture> textures;



			// Walk through each of the mesh's vertices
			for (unsigned int i = 0; i < mesh->mNumVertices; i++)
			{
				Vertex vertex;
				glm::vec3 vector; // we declare a placeholder vector since assimp uses its own vector class that doesn't directly convert to glm's vec3 class so we transfer the data to this placeholder glm::vec3 first.
								  // positions
				vector.x = mesh->mVertices[i].x;
				vector.y = mesh->mVertices[i].y;
				vector.z = mesh->mVertices[i].z;
				vertex.Position = vector;
				// normals
				vector.x = mesh->mNormals[i].x;
				vector.y = mesh->mNormals[i].y;
				vector.z = mesh->mNormals[i].z;
				vertex.Normal = vector;
				// texture coordinates
				if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
				{
					glm::vec2 vec;
					// a vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't 
					// use models where a vertex can have multiple texture coordinates so we always take the first set (0).
					vec.x = mesh->mTextureCoords[0][i].x;
					vec.y = mesh->mTextureCoords[0][i].y;
					vertex.TexCoords = vec;
				}
				else
					vertex.TexCoords = glm::vec2(0.0f, 0.0f);
				// tangent
				vector.x = mesh->mTangents[i].x;
				vector.y = mesh->mTangents[i].y;
				vector.z = mesh->mTangents[i].z;
				vertex.Tangent = vector;
				// bitangent
				vector.x = mesh->mBitangents[i].x;
				vector.y = mesh->mBitangents[i].y;
				vector.z = mesh->mBitangents[i].z;
				vertex.Bitangent = vector;
				vertices.push_back(vertex);



			}
			// now wak through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
			for (unsigned int i = 0; i < mesh->mNumFaces; i++)
			{
				aiFace face = mesh->mFaces[i];
				// retrieve all indices of the face and store them in the indices vector
				for (unsigned int j = 0; j < face.mNumIndices; j++)
					indices.push_back(face.mIndices[j]);
			}
			// process materials
			aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
			// we assume a convention for sampler names in the shaders. Each diffuse texture should be named
			// as 'texture_diffuseN' where N is a sequential number ranging from 1 to MAX_SAMPLER_NUMBER. 
			// Same applies to other texture as the following list summarizes:
			// diffuse: texture_diffuseN
			// specular: texture_specularN
			// normal: texture_normalN

			// 1. diffuse maps
			vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
			textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
			// 2. specular maps
			vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
			textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
			// 3. normal maps
			std::vector<Texture> normalMaps = loadMaterialTextures(material, aiTextureType_NORMALS, "texture_normal");
			textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
			// 4. height maps
			std::vector<Texture> heightMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_height");
			textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

			// return a mesh object created from the extracted mesh data
			return Mesh(vertices, indices, textures);
		}

		// Checks all material textures of a given type and loads the textures if they're not loaded yet.
		// The required info is returned as a Texture struct.
		vector<Texture> Model::loadMaterialTextures(aiMaterial *mat, aiTextureType type, string typeName)
		{
			vector<Texture> textures;
			for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
			{
				aiString str;
				mat->GetTexture(type, i, &str);
				// check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
				bool skip = false;
				for (unsigned int j = 0; j < textures_loaded.size(); j++)
				{
					if (std::strcmp(textures_loaded[j].path.data, str.C_Str()) == 0)
					{
						textures.push_back(textures_loaded[j]);
						skip = true; // a texture with the same filepath has already been loaded, continue to next one. (optimization)
						break;
					}
				}
				if (!skip)
				{   // if texture hasn't been loaded already, load it
					Texture texture;
					texture.id = TextureFromFile(str.C_Str(), this->directory);
					texture.type = typeName;
					texture.path = str.C_Str();
					textures.push_back(texture);
					textures_loaded.push_back(texture);  // store it as texture loaded for entire model, to ensure we won't unnecesery load duplicate textures.
				}
			}
			return textures;
		}


		unsigned int TextureFromFile(const char *path, const string &directory, bool gamma)
		{
			string filename = string(path);
			filename = directory + '/' + filename;

			unsigned int textureID;
			glGenTextures(1, &textureID);

			int width, height, nrComponents;
			unsigned char *data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
			if (data)
			{
				GLenum format;
				if (nrComponents == 1)
					format = GL_RED;
				else if (nrComponents == 3)
					format = GL_RGB;
				else if (nrComponents == 4)
					format = GL_RGBA;

				glBindTexture(GL_TEXTURE_2D, textureID);
				glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
				glGenerateMipmap(GL_TEXTURE_2D);

				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

				stbi_image_free(data);
			}
			else
			{
				std::cout << "Texture failed to load at path: " << path << std::endl;
				stbi_image_free(data);
			}

			return textureID;
		}

		glm::mat4 Model::BoneTransform(aiScene* scene, float TimeInSeconds, std::vector<glm::mat4>& Transforms)
		{
			glm::mat4 identity = glm::mat4();


			float TicksPerSecond = scene->mAnimations[0]->mTicksPerSecond != 0 ?
				scene->mAnimations[0]->mTicksPerSecond : 25.0f;
			float TimeInTicks = TimeInSeconds * TicksPerSecond;
			float AnimationTime = fmod(TimeInTicks, scene->mAnimations[0]->mDuration);

			ReadNodeHeirarchy(AnimationTime, scene, scene->mRootNode, identity);

			Transforms.resize(numBones);

			for (unsigned int i = 0; i < numBones; i++) {
				Transforms[i] = boneTransforms[i];

			}
		}

		void Model::ReadNodeHeirarchy(float AnimationTime, aiScene* scene, aiNode* pNode, const glm::mat4 ParentTransform)
		{
			string nodeName(pNode->mName.data);

			const aiAnimation* animation = scene->mAnimations[0];

			glm::mat4 nodeTransformation = AiToGLMMat4(pNode->mTransformation);

			const aiNodeAnim* nodeAnim = FindAiNodeAnim(nodeName);
			if (animation)
			{
				//Interpolate scaling between frames and generate matrices
				aiVector3D scaling;
				CalcInterpolatedScale(scaling, AnimationTime, nodeAnim);
				glm::mat4 scaleMatrix;
				scaleMatrix = glm::scale(scaleMatrix, glm::vec3(scaling.x, scaling.y, scaling.z));

				//Interpolate rotation between frames and generate matrices
				aiQuaternion rotation;
				CalcInterpolatedRotation(rotation, AnimationTime, nodeAnim);
				glm::mat4 rotationMatrix = glm::mat4(AiToGLMMat4(aiMatrix4x4(rotation.GetMatrix())));

				//Interpolate translation between frames and generate matrices
				aiVector3D translation;
				CalcInterpolatedPosition(translation, AnimationTime, nodeAnim);
				glm::mat4 translationMatrix = glm::translate(translationMatrix,
					glm::vec3(translation.x, translation.y, translation.z));

				//combine transformations
				nodeTransformation = translationMatrix * rotationMatrix * scaleMatrix;


			}



			glm::mat4 globalTransform = ParentTransform * nodeTransformation;

			if (bones.find(nodeName) != bones.end())
			{
				unsigned int boneIndex = bones[nodeName];
				boneTransforms[boneIndex] = globalInverseTransform * globalTransform * boneTransforms[boneIndex];

			}

			for (unsigned int i = 0; i < pNode->mNumChildren; i++)
			{
				ReadNodeHeirarchy(AnimationTime, scene, pNode->mChildren[i], globalTransform);
			}

		}
		void Model::CalcInterpolatedScale (aiVector3D Out, float AnimationTime, const aiNodeAnim * pNodeAnim)
		{
			// we need at least two values to interpolate...
			if (pNodeAnim->mNumScalingKeys == 1) {
				Out = pNodeAnim->mScalingKeys[0].mValue;
				return;
			}

			unsigned int scaleIndex = FindScale(AnimationTime, pNodeAnim);
			unsigned int nextScaleIndex = (scaleIndex + 1);
			assert(nextScaleIndex < pNodeAnim->mNumScalingKeys);
			float DeltaTime = pNodeAnim->mScalingKeys[nextScaleIndex].mTime - pNodeAnim->mScalingKeys[scaleIndex].mTime;
			float Factor = (AnimationTime - (float)pNodeAnim->mScalingKeys[scaleIndex].mTime) / DeltaTime;
			assert(Factor >= 0.0f && Factor <= 1.0f);
			const aiVector3D startScale = pNodeAnim->mScalingKeys[scaleIndex].mValue;
			const aiVector3D endScale = pNodeAnim->mScalingKeys[nextScaleIndex].mValue;
			Out = Out.Normalize;
		}
		void Model::CalcInterpolatedPosition(aiVector3D Out, float AnimationTime, const aiNodeAnim * pNodeAnim)
		{
			// we need at least two values to interpolate...
			if (pNodeAnim->mNumPositionKeys == 1) {
				Out = pNodeAnim->mPositionKeys[0].mValue;
				return;
			}

			unsigned int positionIndex = FindPosition(AnimationTime, pNodeAnim);
			unsigned int nextPositionIndex = (positionIndex + 1);
			assert(nextPositionIndex < pNodeAnim->mNumPositionKeys);
			float DeltaTime = pNodeAnim->mScalingKeys[nextPositionIndex].mTime - pNodeAnim->mPositionKeys[positionIndex].mTime;
			float Factor = (AnimationTime - (float)pNodeAnim->mPositionKeys[positionIndex].mTime) / DeltaTime;
			assert(Factor >= 0.0f && Factor <= 1.0f);
			const aiVector3D startPosition = pNodeAnim->mPositionKeys[positionIndex].mValue;
			const aiVector3D endPosition = pNodeAnim->mPositionKeys[nextPositionIndex].mValue;
			Out = Out.Normalize;
		}
	
		void Model::CalcInterpolatedRotation(aiQuaternion& Out, float AnimationTime, const aiNodeAnim * pNodeAnim)
		{
			// we need at least two values to interpolate...
			if (pNodeAnim->mNumRotationKeys == 1) {
				Out = pNodeAnim->mRotationKeys[0].mValue;
				return;
			}

			unsigned int RotationIndex = FindRotation(AnimationTime, pNodeAnim);
			unsigned int NextRotationIndex = (RotationIndex + 1);
			assert(NextRotationIndex < pNodeAnim->mNumRotationKeys);
			float DeltaTime = pNodeAnim->mRotationKeys[NextRotationIndex].mTime - pNodeAnim->mRotationKeys[RotationIndex].mTime;
			float Factor = (AnimationTime - (float)pNodeAnim->mRotationKeys[RotationIndex].mTime) / DeltaTime;
			assert(Factor >= 0.0f && Factor <= 1.0f);
			const aiQuaternion& StartRotationQ = pNodeAnim->mRotationKeys[RotationIndex].mValue;
			const aiQuaternion& EndRotationQ = pNodeAnim->mRotationKeys[NextRotationIndex].mValue;
			aiQuaternion::Interpolate(Out, StartRotationQ, EndRotationQ, Factor);
			Out = Out.Normalize();
		}

		unsigned int Model::FindScale(float AnimationTime, const aiNodeAnim* pNodeAnim)
		{
			assert(pNodeAnim->mNumScalingKeys > 0);

			for (unsigned int i = 0; i < pNodeAnim->mNumScalingKeys - 1; i++) {
				if (AnimationTime < (float)pNodeAnim->mScalingKeys[i + 1].mTime) {
					return i;
				}
			}

			assert(0);
		}

		unsigned int Model::FindPosition(float AnimationTime, const aiNodeAnim* pNodeAnim)
		{
			assert(pNodeAnim->mNumPositionKeys > 0);

			for (unsigned int i = 0; i < pNodeAnim->mNumPositionKeys - 1; i++) {
				if (AnimationTime < (float)pNodeAnim->mPositionKeys[i + 1].mTime) {
					return i;
				}
			}

			assert(0);
		}

		unsigned int Model::FindRotation(float AnimationTime, const aiNodeAnim* pNodeAnim)
		{
			assert(pNodeAnim->mNumRotationKeys > 0);

			for (unsigned int i = 0; i < pNodeAnim->mNumRotationKeys - 1; i++) {
				if (AnimationTime < (float)pNodeAnim->mRotationKeys[i + 1].mTime) {
					return i;
				}
			}

			assert(0);
		}

		aiNode* Model::FindAiNode(std::string name)
		{
			for (int i = 0; i < ai_nodes.size(); i++)
			{
				if (ai_nodes.at(i)->mName.data == name)
					return ai_nodes.at(i);
			}
			// find aiNode in list find by processNode()
			return nullptr;
		}

		aiNodeAnim* Model::FindAiNodeAnim(std::string name)
		{
			for (int i = 0; i < ai_nodes_anim.size(); i++)
			{
				if (ai_nodes_anim.at(i)->mNodeName.data == name)
					return ai_nodes_anim.at(i);
			}
			// find aiNodeAnim in animNode list
			return nullptr;
		}


	}
}