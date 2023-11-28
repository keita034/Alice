#pragma once
#include<Sprite2D.h>
#include<ITransition.h>

class FadeInTransition :public ITransition
{
private:
	std::unique_ptr<Sprite2D> sprite;
	Transform spriteTransform;
	uint32_t spriteHandle = 0;
	float alpha = 0.0f;

public:

	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Initilize(float windowHeight, float windowWidth)override;

	/// <summary>
	/// 終了
	/// </summary>
	virtual void Finalize()override;

	/// <summary>
	/// 毎フレーム更新
	/// </summary>
	virtual void Update()override;

	/// <summary>
	/// 描画
	/// </summary>
	virtual void Draw(bool draw_)override;

	bool Start()override;

	float GetCoefficient();

	FadeInTransition() = default;
	virtual ~FadeInTransition() = default;

private:
//コピーコンストラクタ・代入演算子削除
	FadeInTransition& operator=(const FadeInTransition&) = delete;
	FadeInTransition(const FadeInTransition&) = delete;
};

