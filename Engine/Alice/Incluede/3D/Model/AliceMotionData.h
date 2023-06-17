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

class MotionData
{
private:
	friend class AliceMotionData;
	friend class AliceFileStream;
	friend class AliceModel;

	std::string filePath;

	uint32_t modelHandle;
	char PADING[4]{};

	//名前
	std::string name;

	//一秒あたりのアニメーション継続時間(指定されてなかったら0)
	float ticksPerSecond;

	//アニメーション時間
	float duration;

	//ノードアニメーションチャンネル
	std::vector<MotionNode> channels;

};

class AliceMotionData
{
private:
	friend class AliceFileStream;
	friend class AliceModel;

	static std::vector<std::string>sFilePaths;

	static std::unordered_map<std::string, std::unique_ptr<MotionData>> sMotionDatas;

	static uint32_t sModelCount;

	static const uint32_t sMAX_MOTION = 1024;

	MotionData* motionData = nullptr;

public:

	float GetAnimeMaxflame()const ;

	float GetTickTimes(float frame_);

	void SetMotion(uint32_t motionHandle_);

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
};

