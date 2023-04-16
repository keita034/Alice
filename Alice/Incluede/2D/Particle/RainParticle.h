#pragma once
#include<Particle.h>

class RainParticle : private Particle
{
private:

	AliceMathF::Matrix4 mTrans, mRot, mScale;

	AliceMathF::Vector2 particleScale = { 2.0f,0.0 };

	AliceMathF::Vector2 particleRotation = { 0.0f,0.0 };


public:

	RainParticle() = default;
	~RainParticle();

	//初期化
	virtual void Initialize()override;

	///<summary>
	///更新
	///</summary>
	virtual void Update(const AliceMathF::Vector3& centerPos, const AliceMathF::Vector2& emitRadius, const AliceMathF::Vector4& col, UINT lifeTime);


	///<summary>
	///ビルボード描画
	///</summary>
	virtual void Draw(Camera* camera, Material* material = nullptr);



private:

	virtual void Update()override;

	/// <summary>
	/// パーティクルの追加
	/// </summary>
	/// <param name="life">生存時間</param>
	/// <param name="position">初期座標</param>
	/// <param name="velocity">速度</param>
	/// <param name="accel">加速度</param>
	/// <param name="scale">{開始時スケール,終了時スケール}</param>
	/// <param name="rotation">{開始時回転角,終了時回転角}</param>
	/// <param name="sColor">開始カラー</param>
	/// <param name="eColor">終了カラー</param>
	virtual void Add(
		UINT life, const AliceMathF::Vector3& position, const AliceMathF::Vector3& velocity,
		const AliceMathF::Vector3& accel, const AliceMathF::Vector2& scale, const AliceMathF::Vector2& rotation
		, const AliceMathF::Vector4& sColor, const AliceMathF::Vector4& eColor)override;

	//コピーコンストラクタ・代入演算子削除
	RainParticle& operator=(const RainParticle&) = delete;
	RainParticle(const RainParticle&) = delete;
};

