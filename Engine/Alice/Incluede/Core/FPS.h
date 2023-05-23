#pragma once
#pragma once
#pragma warning(push)
#pragma warning(disable: 4365)
#pragma warning(disable: 4514)
#pragma warning(disable: 4619)
#pragma warning(disable: 4668)
#pragma warning(disable: 5039)

#include<Windows.h>
#pragma warning(pop)

class FPS
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
public:
	/// <summary>
	/// FPS制御初期化
	/// </summary>
	void FpsControlBegin();

	/// <summary>
	/// FPS制御
	/// </summary>
	void FpsControlEnd();

	/// <summary>
	/// フレームレートを設定
	/// </summary>
	/// <param name="fps_">フレームレート</param>
	void SetFrameRate(float frameRate);

	/// <summary>
	/// フレームレートを取得
	/// </summary>
	/// <returns>フレームレート</returns>
	float GetFrameRate();
};

