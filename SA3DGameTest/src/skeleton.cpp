#include "skeleton.h"

namespace sa3d {
	namespace graphics {
		//Skeleton::Skeleton()
		//{
		//}

		//Skeleton::Skeleton(std::vector<Bone> in_bones, glm::mat4 in_globalInverseTransform)
		//{
		//	Init(in_bones, in_globalInverseTransform);
		//}

		//void Skeleton::Init(std::vector<Bone> in_bones, glm::mat4 in_globalInverseTransform)
		//{
		//	bones = in_bones;
		//	globalInverseTransform = in_globalInverseTransform;

		//	for (int i = 0; i < bones.size(); i++)
		//		bones.at(i).parentSkeleton = this;
		//}

		//Bone* Skeleton::FindBone(std::string name)
		//{
		//	for (int i = 0; i < bones.size(); i++)
		//	{
		//		if (bones.at(i).name == name)
		//			return &bones.at(i);
		//	}
		//	return nullptr;
		//}

		////This one isn't really...
		//void Skeleton::UpdateBoneMatsVector()
		//{
		//	//get bones and update matrix global transforms in order

		//	//If there are no bones, why bother?
		//	if (bones.size() == 0)
		//		return;

		//	//Make sure there's nothning left in the vector.
		//	boneMats.clear();



		//	//Here we must update the matrices to the shader until the MAX_BONES constant
		//	//in the shader, which is 100. You could set this as an engine variable but
		//	//for the purposes of this tutorial simply typing i < 100 will do.
		//	//We do this in order to make certain that every index in the shader's array
		//	//of matrices is filled, that there are no garbage or left-over matrices from
		//	//some other draw call.
		//	for (int i = 0; i < MAX_BONES; i++)
		//	{
		//		//If we are past the number of bones in the actual skeleton, we simply
		//		//pass in an identity matrix.
		//		if (i > bones.size() - 1)
		//			boneMats.push_back(glm::mat4(1.0));
		//		else
		//		{
		//			//Now for the complicated stuff...
		//			//To get the concatenated transformation, we multiply the concatenated transforms of
		//			//the bone's parents with the bone's transformation.
		//			glm::mat4 concatenated_transformation = (bones.at(i).GetParentTransforms() *
		//				AiToGLMMat4(bones.at(i).node->mTransformation));

		//					//DISCLAIMER! By concatenated_transformation, what I actually mean is the transformation of the current bone in the
		//					//skeleton's local (or object) space. At this point it is not in world space, as being in world space requires multiplication by the
		//					//model matrix, which happens only in the shader stage.

		//					//REMEMBER: TO UNDERSTAND MATRIX MULTIPLICATION, WE MUST READ IT BACKWARDS!
		//					//Here we transform the bone back to the origin (with the offset matrix),
		//					//we multiply it by the concatenated transforms,
		//					//and then by the globalInverseTransform.
		//					boneMats.push_back(globalInverseTransform * concatenated_transformation* bones.at(i).offsetMatrix);
		//		}
		//	}
		//}

		//void Skeleton::update()
		//{

		//	UpdateBoneMatsVector();
		//}
	}
}