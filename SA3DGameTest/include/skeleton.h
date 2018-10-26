#pragma once

#ifndef __SKELETON_H__
#define	 __SKELETON_H__

#include "bone.h"

#define MAX_BONES 108
namespace sa3d {
	namespace graphics {

		/**@brief Skeleton class to get skeleton from mesh for animation */
		class Skeleton
		{
		public:
			std::vector<Bone> bones;
			glm::mat4 globalInverseTransform;
			std::vector<glm::mat4> boneMats;

			Skeleton();
			Skeleton(std::vector<Bone> in_bones, glm::mat4 in_globalInverseTransform);


			//Bone* FindBone(std::string name); // find bone in skeleon hierarchy
			/**Update Functions */
			void Init(std::vector<Bone> in_bones, glm::mat4 in_globalInverseTransform);
			void UpdateBoneMatsVector();
			void update();
			

			
		};

	}



}

#endif