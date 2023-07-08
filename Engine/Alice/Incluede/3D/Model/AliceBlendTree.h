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

class AliceBlendTree
{
private:

	BlendNode* startNode;
	BlendNode* endNode;

	ReturnMotionNode zeroReturnMotionNode;
	ReturnMotionNode returnAnimation;

	std::vector< std::unique_ptr<BlendNode>>tree;

	float thresh;
	float localThresh;

	size_t treeIndex = 0;

	bool isDirty = true;

	int8_t PADING[7]{};

public:

	void AddAnimation(uint32_t handle_);

	void SetThresh(float thresh_);

	void Update(float frame_);

	const ReturnMotionNode* GetMotion(const std::string& nodeName);

	AliceBlendTree() = default;
	~AliceBlendTree() = default;

	//コピーコンストラクタ・代入演算子削除
	AliceBlendTree& operator=(const AliceBlendTree&) = delete;
	AliceBlendTree(const AliceBlendTree&) = delete;
};

