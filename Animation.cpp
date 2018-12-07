#include "Animation.h"

#include "GameMaths.h"

void animate(const KmxSkeleton& skeleton, int32 animationIndex, float currentAnimationTime, mat4* inverseBindPoses, mat4** outputPoseMats)
{
	// Invalid animationIndex, set output to bind pose
	if((animationIndex >= skeleton.numAnimations) || (animationIndex < 0))
	{
		// printf("Warning: Invalid animation index %i\n", animationIndex);
		for(uint32 boneIndex = 0; boneIndex < skeleton.numBones; ++boneIndex)
		{
			(*outputPoseMats)[boneIndex] = identity_mat4();
		}
		return;
	}
	
	KmxBone* bones = (KmxBone*)(&skeleton.data + skeleton.bonesOffset);
	KmxAnimation* animations = (KmxAnimation*)(&skeleton.data + skeleton.animationsOffset);
	
	KmxAnimation* animation = &animations[animationIndex];
	assert(currentAnimationTime <= animation->duration);

	KmxBoneKeyFrames* keys = (KmxBoneKeyFrames*)(&skeleton.data + animation->keyFramesOffset);

	for(uint32 boneIndex = 0; boneIndex < skeleton.numBones; ++boneIndex)
	{
		KmxBoneKeyFrames* currBoneKeyFrames = &keys[boneIndex];

		mat4 currBoneTranslation = identity_mat4();
		for(uint32 i = 0; (i < (currBoneKeyFrames->numTraKeys - 1)); ++i)
		{
			float* traKeyTimes = (float*)(&skeleton.data + currBoneKeyFrames->traKeyTimesOffset);

			if(traKeyTimes[i+1] >= currentAnimationTime)
			{
				vec3* traKeys = (vec3*)(&skeleton.data + currBoneKeyFrames->traKeysOffset);
				float keyFrameDuration = traKeyTimes[i+1] - traKeyTimes[i];
				float t = (currentAnimationTime - traKeyTimes[i]) / keyFrameDuration;

				vec3 transFrom = traKeys[i];
				vec3 transTo   = traKeys[i+1];
				vec3 lerpedTrans = transFrom + (transTo-transFrom) * t;
				currBoneTranslation = translate(identity_mat4(), lerpedTrans);
				break;
			}
		}

		mat4 currBoneRotation = identity_mat4();
		for(uint32 i = 0; (i < (currBoneKeyFrames->numRotKeys - 1)); ++i)
		{
			float* rotKeyTimes = (float*)(&skeleton.data + currBoneKeyFrames->rotKeyTimesOffset);

			if(rotKeyTimes[i+1] >= currentAnimationTime)
			{
				versor* rotKeys = (versor*)(&skeleton.data + currBoneKeyFrames->rotKeysOffset);
				float keyFrameDuration = rotKeyTimes[i+1] - rotKeyTimes[i];
				float t = (currentAnimationTime - rotKeyTimes[i]) / keyFrameDuration;

				versor rotateFrom = rotKeys[i];
				versor rotateTo   = rotKeys[i+1];
				versor slerpedRot = slerp(rotateFrom, rotateTo, t);
				currBoneRotation = quat_to_mat4(slerpedRot);
				break;
			}
		}

		KmxBone* currBone = &bones[boneIndex];
		mat4 parentMat = identity_mat4();
		if(currBone->parentIndex >= 0){
			parentMat = (*outputPoseMats)[currBone->parentIndex];
		}
		mat4 currBoneLocalTransform = currBoneTranslation * currBoneRotation;
		mat4 currBonePoseMat = parentMat * currBoneLocalTransform * inverseBindPoses[boneIndex];
		(*outputPoseMats)[boneIndex] = currBonePoseMat;
	}
}
