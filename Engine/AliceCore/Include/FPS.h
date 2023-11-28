#pragma once

#pragma warning(push)
#pragma warning(disable: 4365)
#pragma warning(disable: 4668)
#pragma warning(disable: 4710)
#pragma warning(disable: 4711)
#pragma warning(disable: 4820)
#pragma warning(disable: 5039)

#include<memory>

#pragma warning(pop)

class IFPS
{
public:

	IFPS() = default;
	virtual ~IFPS() = default;

	/// <summary>
	/// FPS制御初期化
	/// </summary>
	virtual void FpsControlBegin() =0;

	/// <summary>
	/// FPS制御
	/// </summary>
	virtual void FpsControlEnd() = 0;

	/// <summary>
	/// フレームレートを設定
	/// </summary>
	/// <param name="fps_">フレームレート</param>
	virtual void SetFrameRate(float frameRate_) = 0;

	/// <summary>
	/// フレームレートを取得
	/// </summary>
	/// <returns>フレームレート</returns>
	virtual float GetFrameRate() = 0;

	virtual float GetDeltaTime() = 0;
};

/// <summary>
/// FPSマネージャーの生成(ユニーク)
/// </summary>
/// <returns>生成されたポインタ</returns>
std::unique_ptr<IFPS> CreateUniqueFPS();

/// <summary>
/// FPSマネージャーの生成(シェアード)
/// </summary>
/// <returns>生成されたポインタ</returns>
std::shared_ptr<IFPS> CreateSharedFPS();