#pragma once
#include<AliceMotionData.h>
#include<AliceBlendTree.h>

class BossAnimation
{
private:

	uint32_t standAnimationHandle;
	uint32_t walkAnimationHandle;
	uint32_t runAnimationHandle;

	uint32_t downAttackAnimationHandle;

	float walkAnimationThresh = 0.0f;

	std::unique_ptr<AliceBlendTree>blendTree;

	float addFrame = 0.02f;

	float standThresh = 0.0f;
	float walkThresh = 0.5f;
	float runThresh = 1.0f;

public:

	BossAnimation() = default;
	~BossAnimation() = default;

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
	AliceBlendTree* GetAnimation()const;

	/// <summary>
	/// アニメーションが進んだの割合を取得
	/// </summary>
	/// <returns></returns>
	float GetRatio();

	void InsertDownAttackAnimation();

	bool IsInsert();

	InsertAnimationPhase GetInsertAnimationPhase();

	void SetAddFrame(float num_ = 0.02f);

	/// <summary>
	/// 歩きモーション関係
	/// </summary>
	void WalkAnimationUpdate(float thresh_);

	void SetStandThresh();
	void SetWalkThresh();
	void SetRunThresh();

private:

	//コピーコンストラクタ・代入演算子削除
	BossAnimation& operator=(const BossAnimation&) = delete;
	BossAnimation(const BossAnimation&) = delete;
};

