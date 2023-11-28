#pragma once
#pragma warning(push)
#pragma warning(disable: 4265)
#pragma warning(disable: 4365)
#pragma warning(disable: 4514)
#pragma warning(disable: 4625)
#pragma warning(disable: 4626)
#pragma warning(disable: 4668)
#pragma warning(disable: 4710)
#pragma warning(disable: 4820)
#pragma warning(disable: 5039)
#pragma warning(disable: 5204)
#pragma warning(disable: 5220)
#pragma warning(disable: 5246)

#include<string>
#include<vector>
#include<stdint.h>
#include<memory>
#pragma warning(pop)

class IAudioManager
{
public:

	virtual void Destroy() = 0;

	virtual void Initialize() = 0;
	virtual void Update() = 0;
	virtual bool NowPlay(uint32_t handle_) = 0;
	virtual void ChangeVolume(uint32_t handle_, float volume_) = 0;
	virtual float GetVolume(uint32_t handle_) = 0;
	virtual uint32_t LoadAudio(const std::string& fileName_, const float& volume_ = 1.0f) = 0;
	virtual int32_t PlayWave(uint32_t handle_, bool loopFlag_ = false) = 0;
	virtual int32_t PlayWaveArray(const std::vector<uint32_t>& handles_) = 0;
	virtual void StopWave(uint32_t handle_) = 0;

	IAudioManager() = default;
	virtual ~IAudioManager() = default;

private:
	//コピーコンストラクタ・代入演算子削除
	IAudioManager& operator=(const IAudioManager&) = delete;
	IAudioManager(const IAudioManager&) = delete;
};

/// <summary>
/// オーディオマネージャーの生成(ユニーク)
/// </summary>
/// <returns>生成されたポインタ</returns>
std::unique_ptr<IAudioManager> CreateUniqueAudioManager();

/// <summary>
/// オーディオマネージャーの生成(シェアード)
/// </summary>
/// <returns>生成されたポインタ</returns>
std::shared_ptr<IAudioManager> CreateSharedAudioManager();
