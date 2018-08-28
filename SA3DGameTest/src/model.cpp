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

			//getAnimations
			processAnim(scene);

			//get global transform for skeleton
			aiMatrix4x4 globalInverseTransform = scene->mRootNode->mTransformation;
			globalInverseTransform.Inverse();

			// process ASSIMP's root node recursively
			processNode(scene->mRootNode, scene);

			for (int i = 0; i < scene->mNumMeshes; i++)
			{
				for (int j = 0; j < scene->mMeshes[i]->mNumBones; j++)
				{
					//Here we're just storing the bone information that we loaded
					//with ASSIMP into the formats our Bone class will recognize.
					std::string b_name = scene->mMeshes[i]->mBones[j]->mName.data;
					glm::mat4 b_mat = glm::transpose(AiToGLMMat4(scene->mMeshes[i]->mBones[j]->mOffsetMatrix));

					//Just because I like debugging...
					std::cout << "Bone " << j << " " << b_name << std::endl;

					//Here we create a Bone Object with the information we've
					//gathered so far, but wait, there's more!
					Bone bone(&meshes.at(i), i, b_name, b_mat);

					//These next parts are simple, we just fill up the bone's
					//remaining data using the functions we defined earlier.
					bone.node = FindAiNode(b_name);
					bone.animNode = FindAiNodeAnim(b_name);

					if (bone.animNode == nullptr)
						std::cout << "No Animations were found for " + b_name << std::endl;

					//Finally, we push the Bone into our vector. Yay.
					bones.push_back(bone);
				}
			}


			//Now we have to fill up the remaining ... remaining data within the
			//bone object, specifically: the pointers to the bone's parent bone.
			for (int i = 0; i < bones.size(); i++)
			{
				//Here we cycle through the existing bones and match them up with
				//their parents, the code here is pretty self explanatory.
				std::string b_name = bones.at(i).name;
				std::string parent_name = FindAiNode(b_name)->mParent->mName.data;

				Bone* p_bone = FindBone(parent_name);

				bones.at(i).parentBone = p_bone;

				if (p_bone == nullptr)
					std::cout << "Parent Bone for " << b_name << " does not exist (is nullptr)" << std::endl;
			}
			//I tried combining the above loop with the one above, but this 
			//only resulted in crashes, and my Just-In-Time debugger isn't working,
			//so I'll just leave it as is.

			//Here we only need to give the first Mesh in meshes the skeleton data
			//because in order to initialize the GameObject that will encapsulate this
			//Mesh, we only need one skeleton. The GameObject will copy the skeleton
			//of the first Mesh in its meshes vector and use this as its own.
			//Did that not make sense?
			//Shit.
			//It will later on though, so don't worry.
			if (meshes.size() > 0)
				meshes->at(0).sceneLoaderSkeleton.Init(bones, globalInverseTransform);
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

		Bone* Model::FindBone(std::string name)
		{
			for (int i = 0; i < bones.size(); i++)
			{
				if (bones.at(i).name == name)
					return &bones.at(i);
			}
			// If bone is not found return null
			return nullptr;
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

		int Model::FindBoneIDByName(std::string name)
		{
			for (int i = 0; i < bones.size(); i++)
			{
				if (bones.at(i).name == name)
					return i;
			}
			// find bone in bones list by name and return the position
			return -1;    
		}

	}
}