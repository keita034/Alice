#pragma once
#pragma warning(push)
#pragma warning(disable: 4365)
#pragma warning(disable: 4514)
#pragma warning(disable: 4619)
#pragma warning(disable: 4668)
#pragma warning(disable: 5039)

#include<unordered_map>
#include<filesystem>

#pragma warning(pop)

#include<AliceMathUtility.h>
#include<AliceUtility.h>

struct ReturnMotionNode
{
	//名前
	std::string name;

	//スケーリング
	AliceMathF::Vector3 scalingKeys;

	//回転角
	AliceMathF::Quaternion rotationKeys;

	//位置
	AliceMathF::Vector3 positionKeys;
};

class MotionData
{
private:
	friend class AliceMotionData;
	friend class AliceFileStream;
	friend class AliceBlendTree;

	//ノードアニメーションチャンネル
	std::vector<MotionNode> channels;

	std::string filePath;

	//名前
	std::string name;

	//一秒あたりのアニメーション継続時間(指定されてなかったら0)
	float ticksPerSecond;

	//アニメーション時間
	float duration;

	uint32_t modelHandle;
	int32_t PADING;

	std::unordered_map<std::string,ReturnMotionNode> returnChannels;
};

class AliceMotionData
{
private:
	friend class AliceFileStream;

	static std::vector<std::string>sFilePaths;

	static std::unordered_map<std::string, std::unique_ptr<MotionData>> sMotionDatas;

	static uint32_t sModelCount;

	static const uint32_t sMAX_MOTION = 1024;

	MotionData* motionData = nullptr;


public:

	float GetAnimeMaxflame()const ;

	float GetTickTimes(float frame_);

	void SetMotion(uint32_t motionHandle_);

	void SetFrame(float frame_);

	const ReturnMotionNode* GetMotion(const std::string& nodeName)const ;

	/// <summary>
	/// モーション生成
	/// </summary>
	/// <param name="filePath">ファイルディレクトリ</param>
	/// <returns>ハンドル</returns>
	static uint32_t SCreateMotion(const std::string& fileDirectoryPath_);

	/// <summary>
	/// 共通初期化
	/// </summary>
	static void SCommonInitialize();

	const MotionData* GetMotionData()const;

private:
	void PCalcInterpolatedScaling(AliceMathF::Vector3& mxOut_,float animationTime_,const MotionNode* pNodeAnim_);
	bool PFindScaling(float animationTime_,const MotionNode* pNodeAnim_,uint32_t& nScalingIndex_);

	void PCalcInterpolatedRotation(AliceMathF::Quaternion& mxOut_,float animationTime_,const MotionNode* pNodeAnim_);
	bool PFindRotation(float animationTime_,const MotionNode* pNodeAnim_,uint32_t& nRotationIndex_);

	void PCalcInterpolatedPosition(AliceMathF::Vector3& mxOut_,float animationTime_,const MotionNode* pNodeAnim_);
	bool PFindPosition(float animationTime_,const MotionNode* pNodeAnim_,uint32_t& nPosIndex_);
};

