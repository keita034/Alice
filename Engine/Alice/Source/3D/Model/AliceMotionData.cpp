#include<AliceMotionData.h>

#pragma warning(push)
#pragma warning(disable: 4365)
#pragma warning(disable: 4514)
#pragma warning(disable: 4619)
#pragma warning(disable: 4668)
#pragma warning(disable: 5039)

#include<atldef.h>

#pragma warning(pop)

#include<FileUtility.h>
#include<AliceFileStream.h>


std::vector<std::string>AliceMotionData::sFilePaths;
std::unordered_map<std::string, std::unique_ptr<MotionData>> AliceMotionData::sMotionDatas;
uint32_t AliceMotionData::sModelCount;

float AliceMotionData::GetAnimeMaxflame()const
{
	return motionData->duration;
}

float AliceMotionData::GetTickTimes(float frame_)
{
	float lTicksPerSecond_ = motionData->ticksPerSecond != 0 ? motionData->ticksPerSecond : 25.0f;

	return  frame_ * lTicksPerSecond_;
}

void AliceMotionData::SetMotion(uint32_t motionHandle_)
{

	motionData = sMotionDatas[sFilePaths[motionHandle_]].get();

	if (!motionData->returnChannels.empty())
	{
		motionData->returnChannels.clear();
	}

	for (MotionNode& node : motionData->channels)
	{
		ReturnMotionNode tmp;
		tmp.name = node.name;
		motionData->returnChannels[node.name] = tmp;
	}
}

void AliceMotionData::SetFrame(float frame_)
{
	float lTicksPerSecond = (motionData->ticksPerSecond != 0 ? motionData->ticksPerSecond : 25.0f);

	float lTimeInTicks = frame_ * lTicksPerSecond;
	float lAnimationTime = fmod(lTimeInTicks, motionData->duration);

	for (MotionNode& node : motionData->channels)
	{
		//スケーリング
		AliceMathF::Vector3 lScaling = {};
		PCalcInterpolatedScaling(lScaling, lAnimationTime, &node);

		//回転角
		AliceMathF::Quaternion lRotation = {};
		PCalcInterpolatedRotation(lRotation, lAnimationTime, &node);

		//移動
		AliceMathF::Vector3 lTranslation = {};
		PCalcInterpolatedPosition(lTranslation, lAnimationTime, &node);

		motionData->returnChannels.at(node.name).scalingKeys = lScaling;
		motionData->returnChannels.at(node.name).rotationKeys = lRotation;
		motionData->returnChannels.at(node.name).positionKeys = lTranslation;
	}
}

const ReturnMotionNode* AliceMotionData::GetMotion(const std::string& nodeName)const
{
	auto lMotionItr = find_if(motionData->returnChannels.begin(), motionData->returnChannels.end(), [&](std::pair<const std::string, ReturnMotionNode>& motion)
		{
			return motion.second.name == nodeName;
		});

	if (lMotionItr == motionData->returnChannels.end())
	{
		return nullptr;
	}
	else
	{
		return &lMotionItr._Ptr->_Myval.second;
	}
}

uint32_t AliceMotionData::SCreateMotion(const std::string& fileDirectoryPath_)
{
	std::string lFilePath;

	if ( fileDirectoryPath_.find(".almb") != std::string::npos || fileDirectoryPath_.find(".alm") != std::string::npos )
	{
		lFilePath = fileDirectoryPath_;
	}
	else
	{
		std::vector<std::string> lFiles;

		lFiles = AliceUtility::Fille::GetFileNames(fileDirectoryPath_);

		//ディレクトリからFBXファイルを探す
		for ( std::string file : lFiles )
		{
			if ( file.find(".almb") != std::string::npos || file.find(".alm") != std::string::npos )
			{
				lFilePath = file;
				break;
			}
		}
	}

	//一回読み込んだことがあるファイルはそのまま返す
	auto lMotionItr = find_if(sMotionDatas.begin(), sMotionDatas.end(), [&](std::pair<const std::string, std::unique_ptr<MotionData, std::default_delete<MotionData>>>& motion)
		{
			return motion.second->filePath == lFilePath;
		});

	if (lMotionItr == sMotionDatas.end())
	{
		uint32_t lModelHandle = sModelCount;

		std::unique_ptr<MotionData> lMotionData;
		lMotionData = std::make_unique<MotionData>();

		{
			std::string lFileExtension = AliceUtility::Fille::FileExtension(lFilePath);

			if (lFileExtension == "almb")
			{
				if (!AliceFileStream::SLoadAliceMotionBinaryData(lFilePath, lMotionData.get()))
				{
					assert(0);
				}
			}
			else if (lFileExtension == "alm")
			{
				if (!AliceFileStream::SLoadAliceMotionData(lFilePath, lMotionData.get()))
				{
					assert(0);
				}
			}
			else
			{
				assert(0);
			}
		}

		lMotionData->modelHandle = sModelCount;

		lMotionData->filePath = lFilePath;

		sMotionDatas[lFilePath] = std::move(lMotionData);

		sFilePaths[sModelCount] = lFilePath;

		sModelCount++;

		return lModelHandle;
	}
	else
	{

		uint32_t lModelHandle = sMotionDatas[lFilePath]->modelHandle;

		return lModelHandle;
	}

	return 0;
}

void AliceMotionData::SCommonInitialize()
{
	sFilePaths.resize(sMAX_MOTION);
}

const MotionData* AliceMotionData::GetMotionData() const
{
	return motionData;
}

void AliceMotionData::PCalcInterpolatedScaling(AliceMathF::Vector3& mxOut_, float animationTime_, const MotionNode* pNodeAnim_)
{
	if (pNodeAnim_->scalingKeys.size() == 1)
	{
		mxOut_ = pNodeAnim_->scalingKeys[0].value;
		return;
	}

	uint32_t lScalingIndex = 0;
	if (!PFindScaling(animationTime_, pNodeAnim_, lScalingIndex))
	{
		mxOut_ = AliceMathF::Vector3(1.0f, 1.0f, 1.0f);
		return;
	}

	uint32_t lNextScalingIndex = (lScalingIndex + 1);
	ATLASSERT(lNextScalingIndex < pNodeAnim_->scalingKeys.size());

	float lDeltaTime = (pNodeAnim_->scalingKeys[lNextScalingIndex].time - pNodeAnim_->scalingKeys[lScalingIndex].time);

	float lFactor = (animationTime_ - pNodeAnim_->scalingKeys[lScalingIndex].time) / lDeltaTime;
	ATLASSERT(lFactor >= 0.0f && lFactor <= 1.0f);

	mxOut_ = AliceMathF::Vector3Lerp(pNodeAnim_->scalingKeys[lScalingIndex].value, pNodeAnim_->scalingKeys[lNextScalingIndex].value, lFactor);
}

bool AliceMotionData::PFindScaling(float animationTime_, const MotionNode* pNodeAnim_, uint32_t& nScalingIndex_)
{
	nScalingIndex_ = 0;
	if (!(pNodeAnim_->scalingKeys.size() > 0))
	{
		return false;
	}

	for (size_t i = 0; i < pNodeAnim_->scalingKeys.size() - 1; i++)
	{
		if ((animationTime_ >= pNodeAnim_->scalingKeys[i].time) && (animationTime_ < pNodeAnim_->scalingKeys[i + 1].time))
		{
			nScalingIndex_ = static_cast<uint32_t>(i);
			return true;
		}
	}

	return false;
}

void AliceMotionData::PCalcInterpolatedRotation(AliceMathF::Quaternion& mxOut_, float animationTime_, const MotionNode* pNodeAnim_)
{
	if (pNodeAnim_->rotationKeys.size() == 1)
	{
		mxOut_ = pNodeAnim_->rotationKeys[0].value;
		return;
	}

	uint32_t lRotationIndex = 0;
	if (!PFindRotation(animationTime_, pNodeAnim_, lRotationIndex))
	{
		mxOut_ = AliceMathF::Quaternion(0.0f, 0.0f, 0.0f, 0.0f);
		return;
	}

	uint32_t lNextRotationIndex = (lRotationIndex + 1);
	ATLASSERT(lNextRotationIndex < pNodeAnim_->rotationKeys.size());

	float lDeltaTime = pNodeAnim_->rotationKeys[lNextRotationIndex].time - pNodeAnim_->rotationKeys[lRotationIndex].time;
	float lFactor = (animationTime_ - pNodeAnim_->rotationKeys[lRotationIndex].time) / lDeltaTime;
	ATLASSERT(lFactor >= 0.0f && lFactor <= 1.0f);

	AliceMathF::QuaternionSlerp(mxOut_, pNodeAnim_->rotationKeys[lRotationIndex].value, pNodeAnim_->rotationKeys[lNextRotationIndex].value, lFactor);
}

bool AliceMotionData::PFindRotation(float animationTime_, const MotionNode* pNodeAnim_, uint32_t& nRotationIndex_)
{
	nRotationIndex_ = 0;
	if (!(pNodeAnim_->rotationKeys.size() > 0))
	{
		return false;
	}

	for (size_t i = 0; i < pNodeAnim_->rotationKeys.size() - 1; i++)
	{

		if ((animationTime_ >= pNodeAnim_->rotationKeys[i].time) && (animationTime_ < pNodeAnim_->rotationKeys[i + 1].time))
		{
			nRotationIndex_ = static_cast<uint32_t>(i);
			return true;
		}
	}

	return false;
}

void AliceMotionData::PCalcInterpolatedPosition(AliceMathF::Vector3& mxOut_, float animationTime_, const MotionNode* pNodeAnim_)
{
	if (pNodeAnim_->positionKeys.size() == 1)
	{
		mxOut_ = pNodeAnim_->positionKeys[0].value;
		return;
	}

	uint32_t lPositionIndex = 0;
	if (!PFindPosition(animationTime_, pNodeAnim_, lPositionIndex))
	{
		mxOut_ = AliceMathF::Vector3(0.0f, 0.0f, 0.0f);
		return;
	}

	uint32_t lNextPositionIndex = (lPositionIndex + 1);

	ATLASSERT(lNextPositionIndex < pNodeAnim_->positionKeys.size());
	float lDeltaTime = (pNodeAnim_->positionKeys[lNextPositionIndex].time - pNodeAnim_->positionKeys[lPositionIndex].time);
	float lFactor = (animationTime_ - pNodeAnim_->positionKeys[lPositionIndex].time) / lDeltaTime;
	ATLASSERT(lFactor >= 0.0f && lFactor <= 1.0f);

	mxOut_ = AliceMathF::Vector3Lerp(pNodeAnim_->positionKeys[lPositionIndex].value, pNodeAnim_->positionKeys[lNextPositionIndex].value, lFactor);
}

bool AliceMotionData::PFindPosition(float animationTime_, const MotionNode* pNodeAnim_, uint32_t& nPosIndex_)
{
	nPosIndex_ = 0;
	if (!(pNodeAnim_->positionKeys.size() > 0))
	{
		return false;
	}

	for (size_t i = 0; i < pNodeAnim_->positionKeys.size() - 1; i++)
	{
		if ((animationTime_ >= pNodeAnim_->positionKeys[i].time) && (animationTime_ < pNodeAnim_->positionKeys[i + 1].time))
		{
			nPosIndex_ = static_cast<uint32_t>(i);
			return true;
		}
	}

	return false;
}