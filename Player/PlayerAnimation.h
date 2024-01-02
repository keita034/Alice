#pragma once
#include<AliceMotionData.h>
#include<AliceBlendTree.h>
#include<Input.h>

class PlayerAnimation
{
private:

	uint32_t standAnimationHandle;
	uint32_t walkAnimationHandle;
	uint32_t runAnimationHandle;

	uint32_t attackAnimationHandle;

	uint32_t attackCombo1AnimationHandle;
	uint32_t attackCombo2AnimationHandle;
	uint32_t attackCombo3AnimationHandle;

	uint32_t rowlingAnimationHandle;
	uint32_t deathAnimationHandle;
	uint32_t hitAnimationHandle;
	uint32_t healingAnimationHandle;

	float walkAnimationThresh =0.0f;

	std::unique_ptr<AliceBlendTree>blendTree;

	AliceInput::IInput* input;

	float addFrame = 0.02f;

public:

	PlayerAnimation() = default;
	~PlayerAnimation() = default;

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(AliceInput::IInput* input_);

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

	void InsertAttackAnimation();
	void InsertAttackCombo1Animation(float frame_);
	void InsertAttackCombo2Animation(float frame_);
	void InsertAttackCombo3Animation(float frame_);
	void InsertRowlingAnimation();
	void InsertDeathAnimation();
	void InsertHitAnimation();
	void InsertHealingAnimation();

	bool IsInsert();

	void AnimationStop();

	void AnimationEndStop();

	InsertAnimationPhase GetInsertAnimationPhase();

	void SetAddFrame(float num_ = 0.02f);

	/// <summary>
	/// 歩きモーション関係
	/// </summary>
	void WalkAnimationUpdate(float thresh_);

private:

	//コピーコンストラクタ・代入演算子削除
	PlayerAnimation& operator=(const PlayerAnimation&) = delete;
	PlayerAnimation(const PlayerAnimation&) = delete;
};

