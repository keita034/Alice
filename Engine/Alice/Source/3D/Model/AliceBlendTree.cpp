#include "AliceBlendTree.h"


void AliceBlendTree::AddAnimation(uint32_t handle_)
{
	std::unique_ptr<BlendNode> node = std::make_unique<BlendNode>();
	node->animationHandle = handle_;
	node->animation = std::make_unique<AliceMotionData>();
	node->animation->SetMotion(handle_);
	node->index = static_cast< uint32_t >( tree.size() );

	tree.push_back(std::move(node));

	isDirty = true;
}

void AliceBlendTree::SetThresh(float thresh_)
{
	thresh = thresh_;

	thresh = AliceMathF::Clamp01(thresh);
}


void AliceBlendTree::Update(float frame_)
{
	float lLength = static_cast< float >( tree.size() );
	float lProgress = ( lLength - 1 ) * thresh;
	float lIndex = std::floor(lProgress);
	float lWeight = lProgress - lIndex;
	
	if ( AliceMathF::Approximately(lWeight,0.0f) && lIndex >= lLength - 1 )
	{
		lIndex = lLength - 2;
		lWeight = 1;
	}

	startNode = tree[ static_cast< size_t >( lIndex ) ].get();
	endNode = tree[ static_cast< size_t >( lIndex + 1 ) ].get();

	localThresh = lWeight;

	if ( isDirty )
	{
		float divNum = 1.0f / static_cast< float >( tree.size() - 1 );
		divNum = AliceMathF::Floor(divNum * 1000.0f) / 1000.0f;

		for ( size_t i = 0; i < tree.size(); i++ )
		{
			if ( i == 0 )
			{
				tree[ i ]->thresh = 0.0f;
			}
			else if ( i == tree.size() - 1 )
			{
				tree[ i ]->thresh = 1.0f;
			}
			else
			{
				tree[ i ]->thresh = divNum * static_cast< float >( i );
			}
		}
	}

	startNode->animation->SetFrame(frame_);
	endNode->animation->SetFrame(frame_);
}

const ReturnMotionNode* AliceBlendTree::GetMotion(const std::string& nodeName)
{
	const ReturnMotionNode* startReturnNode = startNode->animation->GetMotion(nodeName);
	const ReturnMotionNode* endReturnNode = endNode->animation->GetMotion(nodeName);

	if ( !startReturnNode && !endReturnNode )
	{
		return nullptr;
	}
	else if ( !startReturnNode )
	{
		startReturnNode = &zeroReturnMotionNode;
	}
	else if ( !endReturnNode )
	{
		endReturnNode = &zeroReturnMotionNode;
	}

	returnAnimation.name = nodeName;
	returnAnimation.positionKeys = AliceMathF::Vector3Lerp(startReturnNode->positionKeys,endReturnNode->positionKeys,localThresh);
	AliceMathF::QuaternionSlerp(returnAnimation.rotationKeys,startReturnNode->rotationKeys,endReturnNode->rotationKeys,localThresh);
	returnAnimation.scalingKeys = AliceMathF::Vector3Lerp(startReturnNode->scalingKeys,endReturnNode->scalingKeys,localThresh);

	return &returnAnimation;
}
