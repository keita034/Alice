#pragma once
#include<Particle.h>
class FireWorkParticle : public Particle
{
private:

	//基準の座標
	AliceMathF::Vector3 position = { 0,0,0 };
	//開始時カラー
	AliceMathF::Vector4 startColor = { 1.0f,0.0f,0.0f,1.0f };
	//終了時カラー
	AliceMathF::Vector4 endColor = { 1.0f,0.0f,0.0f,1.0f };
	//基準の加速度
	AliceMathF::Vector3 particleAccel = { 0.0f, 0.02f ,0.0f };
	//基準の速度
	AliceMathF::Vector3 particleVelocity = { 1.5f,1.5f,1.5f };
	//スケール(開始時スケール,終了時スケール)
	AliceMathF::Vector2 particleScale = { 10.0f,0.0f };
	//回転角(開始時回転角,終了時回転角)
	AliceMathF::Vector2 particleRotation = { 0.0f,0.0f };

	//基準の生存時間
	uint32_t lifeTime = 50;
	//発生間隔
	size_t intervalTime = 0;
	//一度に発生させる数
	size_t maxIndex = 50;

public:

	FireWorkParticle() = default;
	virtual ~FireWorkParticle() = default;

	//初期化
	virtual void Initialize();

	///<summary>
	///更新
	///</summary>
	virtual void Update();

	///<summary>
	///ビルボード描画
	///</summary>
	virtual void Draw(Camera* camera_,const Material* material_ = nullptr);

	void Add(const AliceMathF::Vector3& pos_);

private:

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
	virtual void PAdd(
		uint32_t life_,
		const AliceMathF::Vector3& position_,
		const AliceMathF::Vector3& velocity_,
		const AliceMathF::Vector3& accel_,
		const AliceMathF::Vector2& scale_,
		const AliceMathF::Vector2& rotation_,
		const AliceMathF::Vector4& sColor_,
		const AliceMathF::Vector4& eColor_);

	//コピーコンストラクタ・代入演算子削除
	FireWorkParticle& operator=(const FireWorkParticle&) = delete;
	FireWorkParticle(const FireWorkParticle&) = delete;
};

