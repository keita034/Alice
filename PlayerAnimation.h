#pragma once
#include<AliceMotionData.h>

class PlayerAnimation
{
private:

	uint32_t standAnimationHandle;
	uint32_t walkAnimationHandle;
	uint32_t runAnimationHandle;

	std::unique_ptr<AliceMotionData>standAnimation;
	std::unique_ptr<AliceMotionData>walkAnimation;
	std::unique_ptr<AliceMotionData>runAnimation;

	float frame = 0.0f;

	AliceMotionData* returnAnimation = nullptr;

public:

	PlayerAnimation() = default;
	~PlayerAnimation() = default;

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// 後始末
	/// </summary>
	void Finalize();

	/// <summary>
	/// 現在アニメーション取得
	/// </summary>
	/// <returns></returns>
	AliceMotionData* GetAnimation()const;

	/// <summary>
	/// 現在フレームを取得
	/// </summary>
	float GetFrame();

private:
	//コピーコンストラクタ・代入演算子削除
	PlayerAnimation& operator=(const PlayerAnimation&) = delete;
	PlayerAnimation(const PlayerAnimation&) = delete;
};

