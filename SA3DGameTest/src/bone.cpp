#include "bone.h"

namespace sa3d {
	namespace graphics {

		Bone::Bone(Mesh* in_mesh, unsigned int in_id, std::string in_name, aiMatrix4x4 in_o_mat)
		{
			id = in_id;
			name = in_name;
			offsetMatrix = AiToGLMMat4(in_o_mat);

			mesh = in_mesh;

			parentBone = nullptr;
			node = nullptr;
		}

		Bone::Bone(Mesh* in_mesh, unsigned int in_id, std::string in_name, glm::mat4 in_o_mat)
		{
			id = in_id;
			name = in_name;
			offsetMatrix = in_o_mat;

			mesh = in_mesh;

			parentBone = nullptr;
			node = nullptr;
		}

		glm::mat4 Bone::GetParentTransforms()
		{
			Bone* b = parentBone;    //In order to recursively concatenate the transforms,
									  //we first start with this bone's parent.
			std::vector<glm::mat4> mats;    //Where we'll store the concatenated transforms.

			while (b != nullptr)    //As long as 'b' has a parent (see the end of the loop
			{                      //to avoid confusion).
				glm::mat4 tmp_mat = AiToGLMMat4(b->node->mTransformation); //This bone's transformation.
				mats.push_back(tmp_mat);

				b = b->parentBone;    //We set b to its own parent so the loop can continue.
			}
			glm::mat4 concatenated_transforms;
			//IMPORTANT!!!! This next loop must be done in reverse, 
			//as multiplication with matrices is not commutative.
			for (int i = mats.size() - 1; i >= 0; i--)
				concatenated_transforms *= mats.at(i);

			return concatenated_transforms;    //Finally, we return the concatenated transforms.
		}


	}
}