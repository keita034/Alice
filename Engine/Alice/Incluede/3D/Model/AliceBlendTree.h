#pragma once
#include<AliceMotionData.h>


struct BlendNode
{
	std::unique_ptr<AliceMotionData> animation;
	uint32_t animationHandle;
	uint32_t index;
	float thresh;

	int32_t PADING;

	BlendNode() = default;
	~BlendNode() = default;

	//コピーコンストラクタ・代入演算子削除
	BlendNode& operator=(const BlendNode&) = delete;
	BlendNode(const BlendNode&) = delete;
};

enum InsertAnimationPhase
{
	BEFORE,
	DURING,
	AFTER
};

class AliceBlendTree
{
private:

	BlendNode* startNode;
	BlendNode* endNode;

	ReturnMotionNode zeroReturnMotionNode;
	ReturnMotionNode returnAnimation;

	std::vector< std::unique_ptr<BlendNode>>tree;

	std::unique_ptr<AliceMotionData> insertAnimation;

	size_t treeIndex = 0;

	InsertAnimationPhase insertAnimationPhase;

	float thresh;
	float localThresh;
	float frame;

	float insertAnimationOneThirdFrame;

	bool isInsert = false;

	bool isDirty = true;

	int8_t PADING[2]{};

public:

	void AddAnimation(uint32_t handle_);

	void InsertAnimation(uint32_t handle_);

	void SetThresh(float thresh_);

	void Update(float addFrameNum_);

	const ReturnMotionNode* GetMotion(const std::string& nodeName);

	bool IsInsert();

	InsertAnimationPhase GetInsertAnimationPhase();

	AliceBlendTree();
	~AliceBlendTree() = default;

private:

	void PInsertBlend(const ReturnMotionNode* animation_,const ReturnMotionNode* insertAnimation_,ReturnMotionNode& returnMotionNode_,float thresh_);

	bool PBlend(const AliceMotionData* startAnimation_,const AliceMotionData* endAnimation_,ReturnMotionNode& returnMotionNode_,const std::string& nodeName);

	void PAnimationChoice();

	//コピーコンストラクタ・代入演算子削除
	AliceBlendTree& operator=(const AliceBlendTree&) = delete;
	AliceBlendTree(const AliceBlendTree&) = delete;
};

