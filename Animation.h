#pragma once

#include "utils.h"

#define MAX_NUM_BONES 100

struct KmxSkinnedMesh {
	uint32 magic;
	uint32 version;

	uint32 indexCount;
	uint32 vertCount;

	uint32 indexOffset;
	uint32 vpOffset;
	uint32 vnOffset;
	uint32 vtOffset;
	uint32 vboneIdOffset;
	uint32 vboneWeightOffset;
	uint32 inverseBindPosesOffset;

	uint8 data;
};

struct KmxSkeleton {
	uint32 magic;
	uint32 version;

	uint32 numBones;
	uint32 bonesOffset;

	uint32 numAnimations;
	uint32 animationsOffset;

	uint8 data;
};

struct KmxBone {
	char name[32];
	int32 parentIndex;
};

struct KmxAnimation {
	char name[32];
	float duration;
	uint32 keyFramesOffset;
};

struct KmxBoneKeyFrames {
	uint32 numTraKeys;
	uint32 numRotKeys;
	// uint32 numScaKeys;

	uint32 traKeyTimesOffset;
	uint32 rotKeyTimesOffset;
	// uint32 scaKeyTimesOffset;

	uint32 traKeysOffset;
	uint32 rotKeysOffset;
	// uint32 scaKeysOffset;
};

struct mat4;

void animate(const KmxSkeleton& skeleton, int32 animationIndex, float currentAnimationTime, mat4* inverseBindPoses, mat4** outputPoseMats);

/*
	KmxSkeleton skel;

	// &skel.data + skel.bonesOffset:
	KmxBone bones[numBones];

	// &skel.data + skel.animationsOffset:
	KmxAnimation anims[numAnimations];

	// &skel.data + anim.keyFramesOffset:
	KmxBoneKeyFrames keys[numAnimations][numBones];

	// &skel.data + key.traKeyTimesOffset:
	float traKeyTimes[numAnimations][numBones][numTraKeys];

	// &skel.data + key.traKeysOffset:
	vec3 traKeys[numAnimations][numBones][numTraKeys];

	// &skel.data + key.rotKeyTimesOffset:
	float rotKeyTimes[numAnimations][numBones][numRotKeys];

	// &skel.data + key.rotKeyTimesOffset:
	versor rotKeys[numAnimations][numBones][numRotKeys];
*/

/* // Skeleton File Layout

	// Skeleton Block: sizeof(KmxSkeleton)
	skeleton

	// Bones Block: (skeleton.numBones * sizeof(KmxBone))
	bone[0], bone[1], ..., bone[N]

	// Animations Block: (skeleton.numAnimations * sizeof(KmxAnimation))
	animation[0], animation[1], ..., animation[N]

	// KeyFrames Block: (skeleton.numAnimations * skeleton.numBones * sizeof(KmxBoneKeyFrames))
	{
		animation[0].boneKeyFrames[0, ..., skeleton.numBones],
		animation[1].boneKeyFrames[0, ..., skeleton.numBones],
		...,
		animation[N].boneKeyFrames[0, ..., skeleton.numBones]
	}

	// TraKeyTimes Block: 
	{
		{
			anim[0].boneKeyFrames[0].traKeyTimes[0, ..., anim[0].boneKeyFrames[0].numTraKeys],
			...,
			anim[0].boneKeyFrames[N].traKeyTimes[0, ..., anim[0].boneKeyFrames[N].numTraKeys]
		},
		...,
		{
			anim[N].boneKeyFrames[0].traKeyTimes[0, ..., anim[N].boneKeyFrames[0].numTraKeys],
			...,
			anim[N].boneKeyFrames[N].traKeyTimes[0, ..., anim[N].boneKeyFrames[N].numTraKeys]
		}
	}

	// TraKeys Block:
	{
		anim[0].boneKeyFrames[0].traKeys[0, ..., anim[0].boneKeyFrames[0].numTraKeys]
		...,
		anim[0].boneKeyFrames[N].traKeys[0, ..., anim[0].boneKeyFrames[N].numTraKeys]
	},
	...,
	{
		anim[N].boneKeyFrames[0].traKeys[0, ..., anim[N].boneKeyFrames[0].numTraKeys]
		...,
		anim[N].boneKeyFrames[N].traKeys[0, ..., anim[N].boneKeyFrames[N].numTraKeys]
	}
*/