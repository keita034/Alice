#include<AliceBlendTree.h>

void AliceBlendTree::AddAnimation(uint32_t handle_)
{
	std::unique_ptr<BlendNode> node = std::make_unique<BlendNode>();
	node->animationHandle = handle_;
	node->animation = std::make_unique<AliceMotionData>();
	node->animation->SetMotion(handle_);
	node->index = static_cast<uint32_t>(tree.size());

	tree.push_back(std::move(node));

	isDirty = true;
}

bool AliceBlendTree::InsertAnimation(uint32_t handle_, bool complement_)
{
	if (!isInsert|| animationEndStop && !isPlay)
	{
		insertAnimation->SetMotion(handle_);
		frame = 0.0f;
		isInsert = true;
		insertAnimationPhase = BEFORE;
		insertAnimationOneThirdFrame = insertAnimation->GetAnimeMaxflame() / 3.0f;
		complement = complement_;

		return true;
	}

	return false;

}

void AliceBlendTree::SetThresh(float thresh_)
{
	thresh = thresh_;

	thresh = AliceMathF::Clamp01(thresh);
}


void AliceBlendTree::Update(float addFrameNum_)
{
	if (isPlay)
	{
		if (!animationEndStop)
		{
			frame += addFrameNum_;
		}
		else
		{
			if (isInsert)
			{
				float lTmpframe = frame + addFrameNum_;

				if (insertAnimation->GetAnimeMaxflame() < insertAnimation->GetTickTimes(lTmpframe))
				{
					isPlay = false;
				}
				else
				{
					frame = lTmpframe;
				}
			}
		}

	}

	if (!isInsert)
	{
		PAnimationChoice();

		if (animationEndStop && isPlay)
		{
			float lTmpframe = frame = addFrameNum_;

			if (startNode->animation->GetAnimeMaxflame() < startNode->animation->GetTickTimes(lTmpframe))
			{
				isPlay = false;
			}
			else
			{
				frame = lTmpframe;
			}
		}

		startNode->animation->SetFrame(frame);
		endNode->animation->SetFrame(frame);
	}
	else
	{
		insertAnimation->SetFrame(frame);

		if (insertAnimationOneThirdFrame < insertAnimation->GetTickTimes(frame))
		{
			insertAnimationPhase = DURING;
		}

		if (insertAnimationOneThirdFrame * 2.0f < insertAnimation->GetTickTimes(frame))
		{
			insertAnimationPhase = AFTER;
		}

		if (insertAnimationPhase == BEFORE || insertAnimationPhase == AFTER)
		{
			PAnimationChoice();
		}

		if (insertAnimation->GetAnimeMaxflame() < insertAnimation->GetTickTimes(frame))
		{
			frame = 0.0f;
			isInsert = false;

		}

	}
}

const ReturnMotionNode* AliceBlendTree::GetMotion(const std::string& nodeName)
{
	ReturnMotionNode lReturnNode;

	if (!isInsert)
	{
		if (!PBlend(startNode->animation.get(), endNode->animation.get(), lReturnNode, nodeName))
		{
			return nullptr;
		}
	}
	else
	{
		if (insertAnimationPhase == BEFORE)
		{
			ReturnMotionNode lAnimation;

			const ReturnMotionNode* lInsert = insertAnimation->GetMotion(nodeName);

			if (!PBlend(startNode->animation.get(), endNode->animation.get(), lAnimation, nodeName))
			{
				if (lInsert)
				{
					lAnimation = zeroReturnMotionNode;
				}
				else
				{
					return nullptr;
				}
			}

			if (!lInsert)
			{
				lInsert = &zeroReturnMotionNode;
			}

			float lThresh = insertAnimation->GetTickTimes(frame) / insertAnimationOneThirdFrame;

			PInsertBlend(&lAnimation, lInsert, lReturnNode, lThresh);
		}
		else if (insertAnimationPhase == AFTER)
		{
			if (complement)
			{

				ReturnMotionNode lAnimation;

				const ReturnMotionNode* lInsert = insertAnimation->GetMotion(nodeName);

				if (!PBlend(startNode->animation.get(), endNode->animation.get(), lAnimation, nodeName))
				{
					if (lInsert)
					{
						lAnimation = zeroReturnMotionNode;
					}
					else
					{
						return nullptr;
					}
				}

				if (!lInsert)
				{
					lInsert = &zeroReturnMotionNode;
				}

				float lTickTime = insertAnimation->GetTickTimes(frame);
				float lThresh = (lTickTime - insertAnimationOneThirdFrame * 2.0f) / insertAnimationOneThirdFrame;

				PInsertBlend(lInsert, &lAnimation, lReturnNode, lThresh);
			}
			else
			{
				const ReturnMotionNode* insertReturnNode = insertAnimation->GetMotion(nodeName);

				if (!insertReturnNode)
				{
					return nullptr;
				}

				lReturnNode.name = nodeName;
				lReturnNode.positionKeys = insertReturnNode->positionKeys;
				lReturnNode.rotationKeys = insertReturnNode->rotationKeys;
				lReturnNode.scalingKeys = insertReturnNode->scalingKeys;
			}

		}
		else
		{
			const ReturnMotionNode* insertReturnNode = insertAnimation->GetMotion(nodeName);

			if (!insertReturnNode)
			{
				return nullptr;
			}

			lReturnNode.name = nodeName;
			lReturnNode.positionKeys = insertReturnNode->positionKeys;
			lReturnNode.rotationKeys = insertReturnNode->rotationKeys;
			lReturnNode.scalingKeys = insertReturnNode->scalingKeys;
		}
	}

	returnAnimation = lReturnNode;

	return &returnAnimation;
}

float AliceBlendTree::GetRatio()
{
	if (!isInsert)
	{
		std::unique_ptr<AliceMotionData>& node = startNode->animation;

		return node->GetTickTimes(frame) / node->GetAnimeMaxflame();;
	}
	else
	{
		return  insertAnimation->GetTickTimes(frame) / insertAnimation->GetAnimeMaxflame();
	}
}

bool AliceBlendTree::IsInsert()
{
	//次フレームでは終わってるため
	if (animationEndStop && !isPlay)
	{
		return false;
	}

	return isInsert;
}

InsertAnimationPhase AliceBlendTree::GetInsertAnimationPhase()
{
	return insertAnimationPhase;
}

void AliceBlendTree::Stop()
{
	isPlay = false;
}

void AliceBlendTree::Start()
{
	isPlay = true;
	animationEndStop = false;
}

void AliceBlendTree::AnimationEndStop()
{
	animationEndStop = true;
}

AliceBlendTree::AliceBlendTree()
{
	insertAnimation = std::make_unique<AliceMotionData>();
	zeroReturnMotionNode.rotationKeys = zeroReturnMotionNode.rotationKeys.Identity();
	zeroReturnMotionNode.scalingKeys = { 1.0f,1.0f,1.0f };
}

void AliceBlendTree::PAnimationChoice()
{
	float lLength = static_cast<float>(tree.size());
	float lProgress = (lLength - 1) * thresh;
	float lIndex = std::floor(lProgress);
	float lWeight = lProgress - lIndex;

	if (AliceMathF::Approximately(lWeight, 0.0f) && lIndex >= lLength - 1)
	{
		lIndex = lLength - 2;
		lWeight = 1;
	}

	startNode = tree[static_cast<size_t>(lIndex)].get();
	endNode = tree[static_cast<size_t>(lIndex + 1)].get();

	localThresh = lWeight;
}

void AliceBlendTree::PInsertBlend(const ReturnMotionNode* animation_, const ReturnMotionNode* insertAnimation_, ReturnMotionNode& returnMotionNode_, float thresh_)
{
	float lThresh = AliceMathF::Clamp01(thresh_);

	returnMotionNode_.positionKeys = AliceMathF::Vector3Lerp(animation_->positionKeys, insertAnimation_->positionKeys, lThresh);
	AliceMathF::QuaternionSlerp(returnMotionNode_.rotationKeys, animation_->rotationKeys, insertAnimation_->rotationKeys, lThresh);
	returnMotionNode_.scalingKeys = AliceMathF::Vector3Lerp(animation_->scalingKeys, insertAnimation_->scalingKeys, lThresh);
}

bool AliceBlendTree::PBlend(const AliceMotionData* startAnimation_, const AliceMotionData* endAnimation_, ReturnMotionNode& returnMotionNode_, const std::string& nodeName)
{
	const ReturnMotionNode* startReturnNode = startAnimation_->GetMotion(nodeName);
	const ReturnMotionNode* endReturnNode = endAnimation_->GetMotion(nodeName);

	if (!startReturnNode && !endReturnNode)
	{
		return false;
	}
	else if (!startReturnNode)
	{
		startReturnNode = &zeroReturnMotionNode;
	}
	else if (!endReturnNode)
	{
		endReturnNode = &zeroReturnMotionNode;
	}

	returnMotionNode_.name = nodeName;
	returnMotionNode_.positionKeys = AliceMathF::Vector3Lerp(startReturnNode->positionKeys, endReturnNode->positionKeys, localThresh);
	AliceMathF::QuaternionSlerp(returnMotionNode_.rotationKeys, startReturnNode->rotationKeys, endReturnNode->rotationKeys, localThresh);
	returnMotionNode_.scalingKeys = AliceMathF::Vector3Lerp(startReturnNode->scalingKeys, endReturnNode->scalingKeys, localThresh);

	return true;
}
