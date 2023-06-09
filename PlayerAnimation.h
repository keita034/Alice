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
	uint32_t rowlingAnimationHandle;

	float frame = 0.0f;

	float walkAnimationThresh =0.0f;

	std::unique_ptr<AliceBlendTree>blendTree;

	AliceInput::IInput* input;

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
	/// 現在フレームを取得
	/// </summary>
	float GetFrame();

	void InsertAttackAnimation();

	void InsertRowlingAnimation();


private:

	/// <summary>
	/// 歩きモーション関係
	/// </summary>
	void PWalkAnimationUpdate();

	void PAttackAnimationUpdate();

	//コピーコンストラクタ・代入演算子削除
	PlayerAnimation& operator=(const PlayerAnimation&) = delete;
	PlayerAnimation(const PlayerAnimation&) = delete;
};

