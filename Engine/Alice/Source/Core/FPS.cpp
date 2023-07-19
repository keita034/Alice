#include<FPS.h>

#pragma warning(push)
#pragma warning(disable: 4365)
#pragma warning(disable: 4514)
#pragma warning(disable: 4619)
#pragma warning(disable: 4668)
#pragma warning(disable: 5039)

#include<Windows.h>
#pragma warning(pop)

class FPS : public IFPS
{
private:
	//CPU周波数
	LARGE_INTEGER cpuClock{};
	//計測開始時間
	LARGE_INTEGER timeStart{};
	//計測終了時間
	LARGE_INTEGER timeEnd{};
	//固定する時間
	float frameTime = 1 / 60.0f;
	//FPS値
	float fps;
	float deltaTime;

	int32_t PADING;

public:
	/// <summary>
	/// FPS制御初期化
	/// </summary>
	void FpsControlBegin()override;

	/// <summary>
	/// FPS制御
	/// </summary>
	void FpsControlEnd()override;

	/// <summary>
	/// フレームレートを設定
	/// </summary>
	/// <param name="fps_">フレームレート</param>
	void SetFrameRate(float frameRate_)override;

	/// <summary>
	/// フレームレートを取得
	/// </summary>
	/// <returns>フレームレート</returns>
	float GetFrameRate()override;

	float GetDeltaTime()override;

	FPS() = default;
	~FPS() = default;
};

void FPS::FpsControlBegin()
{
	//周波数取得
	QueryPerformanceFrequency(&cpuClock);
	//計測開始時間の初期化
	QueryPerformanceCounter(&timeStart);
}

void FPS::FpsControlEnd()
{
	//今の時間を取得
	QueryPerformanceCounter(&timeEnd);
	//経過時間
	deltaTime = static_cast<float>(timeEnd.QuadPart - timeStart.QuadPart)/static_cast<float>(cpuClock.QuadPart);
	//余裕があるときは待つ
	if ( deltaTime < frameTime)
	{
		//sleep時間
		DWORD sleepTime = static_cast<DWORD>((frameTime - deltaTime ) * 1000.0f);
		timeBeginPeriod(1);
		//寝る
		Sleep(sleepTime);
		timeEndPeriod(1);
	}

	fps = 1 / deltaTime;
}

void FPS::SetFrameRate(float frameRate_)
{
	frameTime = 1.0f / frameRate_;
}

float FPS::GetFrameRate()
{
	return fps;
}

float FPS::GetDeltaTime()
{
	return deltaTime;
}

std::unique_ptr<IFPS> CreateUniqueFPS()
{
	std::unique_ptr<IFPS>lFps = std::make_unique<FPS>();
	return std::move(lFps);
}

std::shared_ptr<IFPS> CreateSharedFPS()
{
	std::shared_ptr<IFPS> lFps = std::make_unique<FPS>();
	return lFps;
}
