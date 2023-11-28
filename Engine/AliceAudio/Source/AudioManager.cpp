#include<AudioManager.h>

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

#include<xaudio2.h>
#include<mfapi.h>
#include<mfidl.h>
#include<mfreadwrite.h>
#include<wrl.h>
#include<list>
#include<cassert>
#include<filesystem>

#pragma warning(pop)

std::wstring StrToWstr(const std::string& string_)
{
	auto const lDestSize = ::MultiByteToWideChar(CP_ACP, 0U, string_.data(), -1, nullptr, 0U);
	std::vector<wchar_t> lDest(static_cast<const uint64_t>(lDestSize), L'\0');

	if (::MultiByteToWideChar(CP_ACP, 0U, string_.data(), -1, lDest.data(), static_cast<int32_t>(lDest.size())) == 0)
	{
		throw std::system_error{ static_cast<int32_t>(::GetLastError()), std::system_category() };
	}

	lDest.resize(std::char_traits<wchar_t>::length(lDest.data()));
	lDest.shrink_to_fit();

	return std::wstring(lDest.begin(), lDest.end());
}

class AudioData
{
public:
	std::string filePass;
	IMFSourceReader* pMFSourceReader = nullptr;
	WAVEFORMATEX* waveFormat = nullptr;
	std::vector<uint8_t> mediaData;
	IXAudio2SourceVoice* pSourceVoice = nullptr;
	float volume = 1.0f;
	bool loop = false;
	bool playTrigger = false;
	char PADDING2[2]{};
	AudioData(const std::string& filePass_);
	void Unload();
};

class XAudio2VoiceCallback : public IXAudio2VoiceCallback
{
public:

	virtual ~XAudio2VoiceCallback()
	{
	};

	//ボイス処理パスの開始時
	STDMETHOD_(void, OnVoiceProcessingPassStart)(THIS_ uint32_t bytesRequired_)
	{
		static_cast<void>(bytesRequired_);
	};

	//ボイス処理パスの終了時
	STDMETHOD_(void, OnVoiceProcessingPassEnd) (THIS)
	{
	};

	//バッファストリームの再生が終了したとき
	STDMETHOD_(void, OnStreamEnd) (THIS)
	{
	};

	//バッファの使用開始時
	STDMETHOD_(void, OnBufferStart) (THIS_ void* pBufferContext_)
	{
		static_cast<void>(pBufferContext_);

	};

	//バッファの末尾に達した時
	STDMETHOD_(void, OnBufferEnd) (THIS_ void* pBufferContext_)
	{
		static_cast<void>(pBufferContext_);
	};

	//再生がループ位置に達した時
	STDMETHOD_(void, OnLoopEnd) (THIS_ void* pBufferContext_)
	{
		static_cast<void>(pBufferContext_);
	};

	//ボイスの実行エラー時
	STDMETHOD_(void, OnVoiceError) (THIS_ void* pBufferContext_, HRESULT error_)
	{
		static_cast<void>(pBufferContext_);
		static_cast<void>(error_);
	};
};

struct PlayAudioArray
{
	std::vector<uint32_t>handles;
	int32_t nowIdx = 0;
	int32_t PADING;
	PlayAudioArray(const std::vector<uint32_t>& Handles_);
};

class AudioManager : public IAudioManager
{
private:
	Microsoft::WRL::ComPtr<IXAudio2>xAudio2;
	IXAudio2MasteringVoice* masterVoice = nullptr;
	XAudio2VoiceCallback voiceCallback;
	std::list<AudioData>audios;
	std::vector<PlayAudioArray>playHandleArray;

public:

	void Destroy()override;

	void Initialize()override;
	void Update()override;
	bool NowPlay(uint32_t handle_)override;
	void ChangeVolume(uint32_t handle_, float volume_)override;
	float GetVolume(uint32_t handle_)override;
	uint32_t LoadAudio(const std::string& fileName_, const float& volume_ = 1.0f)override;
	int32_t PlayWave(uint32_t handle_, bool loopFlag_ = false)override;
	int32_t PlayWaveArray(const std::vector<uint32_t>& handles_)override;
	void StopWave(uint32_t handle_)override;

	AudioManager();
	~AudioManager();

private:
	//コピーコンストラクタ・代入演算子削除
	AudioManager& operator=(const AudioManager&) = delete;
	AudioManager(const AudioManager&) = delete;
};


AudioManager::AudioManager()
{

}


AudioManager::~AudioManager()
{
	xAudio2.Reset();

	std::list<AudioData>::iterator lAudioItr = audios.begin();

	for (; lAudioItr != audios.end(); ++lAudioItr)
	{
		lAudioItr->Unload();
	}
}

void AudioManager::Destroy()
{
	MFShutdown();
}

void AudioManager::Initialize()
{
	HRESULT lResult;

	MFStartup(MF_VERSION, MFSTARTUP_NOSOCKET);

	lResult = XAudio2Create(&xAudio2, 0);
	assert(SUCCEEDED(lResult));

#if defined(_DEBUG)
	XAUDIO2_DEBUG_CONFIGURATION lDebug{ 0 };
	lDebug.TraceMask = XAUDIO2_LOG_ERRORS | XAUDIO2_LOG_WARNINGS;
	lDebug.BreakMask = XAUDIO2_LOG_ERRORS;
	xAudio2->SetDebugConfiguration(&lDebug, 0);
#endif

	//マスターボイスを生成
	lResult = xAudio2->CreateMasteringVoice(&masterVoice);
	assert(SUCCEEDED(lResult));
}

void AudioManager::Update()
{
	for (AudioData& audio : audios)
	{
		audio.playTrigger = false;
	}

	if (!playHandleArray.empty())
	{
		for (PlayAudioArray& audioArray : playHandleArray)
		{
			if (NowPlay(audioArray.handles[static_cast<const unsigned _int64>(audioArray.nowIdx)]))
			{
				audioArray.nowIdx++;

				if (static_cast<uint32_t>(audioArray.nowIdx) < audioArray.handles.size())
				{
					PlayWave(audioArray.handles[static_cast<const unsigned _int64>(audioArray.nowIdx)]);
				}
			}
		}

		//全部再生しきったら削除
		std::vector<PlayAudioArray>::iterator lAudioItr = playHandleArray.begin();
		for (; lAudioItr != playHandleArray.end();)
		{
			if (lAudioItr->handles.size() <= static_cast<uint32_t>(lAudioItr->nowIdx))
			{
				lAudioItr = playHandleArray.erase(lAudioItr);
			}
			else
			{
				++lAudioItr;
			}
		}
	}
}

bool AudioManager::NowPlay(uint32_t handle_)
{
	std::list<AudioData>::iterator lAudioItr = audios.begin();

	for (size_t i = 0; i < handle_; i++)
	{
		lAudioItr++;
	}

	if (lAudioItr->pSourceVoice != nullptr)
	{
		XAUDIO2_VOICE_STATE lState;

		lAudioItr->pSourceVoice->GetState(&lState);

		return !(lState.pCurrentBufferContext == nullptr);
	}
	else
	{
		return false;
	}
}

void AudioManager::ChangeVolume(uint32_t handle_, float volume_)
{
	std::list<AudioData>::iterator lAudioItr = audios.begin();

	for (size_t i = 0; i < handle_; i++)
	{
		lAudioItr++;
	}

	lAudioItr->volume = volume_;

	if (lAudioItr->pSourceVoice != nullptr)
	{
		lAudioItr->pSourceVoice->SetVolume(lAudioItr->volume);
	}
}

float AudioManager::GetVolume(uint32_t handle_)
{
	std::list<AudioData>::iterator lAudioItr = audios.begin();

	for (size_t i = 0; i < handle_; i++)
	{
		lAudioItr++;
	}

	return lAudioItr->volume;
}

uint32_t AudioManager::LoadAudio(const std::string& fileName_, const float& volume_)
{
	HRESULT lResult = 0;

	if (!audios.empty())
	{
		uint32_t i = 0;
		for (auto lAudioItr = audios.begin(); lAudioItr != audios.end(); lAudioItr++)
		{
			if (lAudioItr->filePass == fileName_)
			{
				return i;
			}
			i++;
		}
	}

	audios.emplace_back(fileName_);

	std::wstring lWFilePath = StrToWstr(fileName_);

	lResult = MFCreateSourceReaderFromURL(lWFilePath.c_str(), NULL, &audios.back().pMFSourceReader);

	IMFMediaType*lMFMediaType = nullptr;
	MFCreateMediaType(&lMFMediaType);
	lMFMediaType->SetGUID(MF_MT_MAJOR_TYPE, MFMediaType_Audio);
	lMFMediaType->SetGUID(MF_MT_SUBTYPE, MFAudioFormat_PCM);
	audios.back().pMFSourceReader->SetCurrentMediaType(static_cast<DWORD>(MF_SOURCE_READER_FIRST_AUDIO_STREAM), nullptr, lMFMediaType);

	lMFMediaType->Release();
	lMFMediaType = nullptr;
	audios.back().pMFSourceReader->GetCurrentMediaType(static_cast<DWORD>(MF_SOURCE_READER_FIRST_AUDIO_STREAM), &lMFMediaType);

	MFCreateWaveFormatExFromMFMediaType(lMFMediaType, &audios.back().waveFormat, nullptr);

	while (true)
	{
		IMFSample* lMFSample = nullptr;
		DWORD lDwStreamFlags{ 0 };
		audios.back().pMFSourceReader->ReadSample(static_cast<DWORD>(MF_SOURCE_READER_FIRST_AUDIO_STREAM), 0, nullptr, &lDwStreamFlags, nullptr, &lMFSample);

		if (lDwStreamFlags & MF_SOURCE_READERF_ENDOFSTREAM)
		{
			break;
		}

		IMFMediaBuffer* lMFMediaBuffer= nullptr ;
		lMFSample->ConvertToContiguousBuffer(&lMFMediaBuffer);

		BYTE* lBuffer{ nullptr };
		DWORD lCbCurrentlength{ 0 };
		lMFMediaBuffer->Lock(&lBuffer, nullptr, &lCbCurrentlength);

		audios.back().mediaData.resize(audios.back().mediaData.size() + lCbCurrentlength);
		memcpy(audios.back().mediaData.data() + audios.back().mediaData.size() - lCbCurrentlength, lBuffer, lCbCurrentlength);

		lMFMediaBuffer->Unlock();

		lMFMediaBuffer->Release();
		lMFSample->Release();
	}

	uint32_t lHandle = static_cast<uint32_t>(audios.size() - 1);

	ChangeVolume(lHandle, volume_);

	return lHandle;
}

int32_t AudioManager::PlayWave(uint32_t handle_, bool loopFlag_)
{
	HRESULT lResult;

	std::list<AudioData>::iterator lAudioItr = audios.begin();
	for (size_t i = 0; i < handle_; i++)
	{
		lAudioItr++;
	}

	//同時に同じ音源を再生しない
	if (lAudioItr->playTrigger)
	{
		return -1;
	}

	//ループ再生で再生しようとしたら既に流れているものを停止
	if (loopFlag_ && NowPlay(handle_))
	{
		lAudioItr->pSourceVoice->Stop();
	}


	xAudio2->CreateSourceVoice(&lAudioItr->pSourceVoice, lAudioItr->waveFormat);

	XAUDIO2_BUFFER lBuffer{ 0 };
	lBuffer.pAudioData = lAudioItr->mediaData.data();
	lBuffer.Flags = XAUDIO2_END_OF_STREAM;
	lBuffer.AudioBytes = sizeof(BYTE) * static_cast<UINT32>(lAudioItr->mediaData.size());

	if (loopFlag_)
	{
		lBuffer.LoopCount = XAUDIO2_LOOP_INFINITE;
	}

	//音量の設定
	lAudioItr->pSourceVoice->SetVolume(lAudioItr->volume);

	//波形データの再生
	lResult = lAudioItr->pSourceVoice->SubmitSourceBuffer(&lBuffer);
	lResult = lAudioItr->pSourceVoice->Start();

	lAudioItr->playTrigger = true;

	return 1;
}

int32_t AudioManager::PlayWaveArray(const std::vector<uint32_t>& handles_)
{
	if (handles_.empty())
	{
		return 0;
	}

	playHandleArray.emplace_back(handles_);
	return PlayWave(handles_[0]);
};

void AudioManager::StopWave(uint32_t handle_)
{
	std::list<AudioData>::iterator lAudioItr = audios.begin();

	for (size_t i = 0; i < handle_; i++)
	{
		lAudioItr++;
	}

	if (lAudioItr->pSourceVoice != nullptr)
	{
		lAudioItr->pSourceVoice->Stop();
		lAudioItr->pSourceVoice->FlushSourceBuffers();
	}
}

AudioData::AudioData(const std::string& filePass_) :filePass(filePass_)
{
}

void AudioData::Unload()
{
}

PlayAudioArray::PlayAudioArray(const std::vector<uint32_t>& handles_) :handles(handles_)
{
}

std::unique_ptr<IAudioManager> CreateUniqueAudioManager()
{
	std::unique_ptr<IAudioManager>lAudioManager = std::make_unique<AudioManager>();
	lAudioManager->Initialize();
	return std::move(lAudioManager);
}

std::shared_ptr<IAudioManager> CreateSharedAudioManager()
{
	std::shared_ptr<IAudioManager> lAudioManager = std::make_unique<AudioManager>();
	lAudioManager->Initialize();
	return lAudioManager;
}
