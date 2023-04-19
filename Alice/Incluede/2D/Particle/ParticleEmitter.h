#pragma once
#include<AliceMathUtility.h>
#include<BasicParticle.h>
#include<TextureManager.h>

class ParticleEmitter
{
private:

	//基準の座標
	AliceMathF::Vector3 position = { 0,0,0 };
	//開始時カラー
	AliceMathF::Vector4 startColor = { 1.0f,0.0f,0.0f,1.0f };
	//終了時カラー
	AliceMathF::Vector4 endColor = { 0.0f,0.0f,1.0f,1.0f };
	//基準の加速度
	AliceMathF::Vector3 particleAccel = {0.0f, 0.001f ,0.0f};
	//基準の速度
	AliceMathF::Vector3 particleVelocity = { 0.1f,0.1f,0.1f };
	//スケール(開始時スケール,終了時スケール)
	AliceMathF::Vector2 particleScale = {1.0f,0.0f};
	//回転角(開始時回転角,終了時回転角)
	AliceMathF::Vector2 particleRotation = {0.0f,0.0f};

	//基準の生存時間
	UINT lifeTime = 50;
	//最大発生間隔
	size_t maxIntervalTime = 60;
	//発生間隔
	size_t intervalTime = 0;
	//一度に発生させる数
	size_t maxIndex = 50;

	//パーティクル
	std::unique_ptr<BasicParticle> particle;

	TextureData* textureData;

public:

	/// <summary>
	/// 基準の座標を設定
	/// </summary>
	/// <param name="pos">座標</param>
	void SetPosition(const AliceMathF::Vector3& pos);

	/// <summary>
	/// 開始時カラーを設定
	/// </summary>
	/// <param name="color">カラー</param>
	void SetStartColor(const AliceMathF::Vector4& color);

	/// <summary>
	/// 終了時カラーを設定
	/// </summary>
	/// <param name="color">カラー</param>
	void SetEndColor(const AliceMathF::Vector4& color);

	/// <summary>
	/// 基準の加速度を設定
	/// </summary>
	/// <param name="accel">加速度</param>
	void SetAccel(const AliceMathF::Vector3& accel);

	/// <summary>
	/// 基準の速度を設定
	/// </summary>
	/// <param name="velocity">速度</param>
	void SetVelocity(const AliceMathF::Vector3& velocity);

	/// <summary>
	/// スケールを設定
	/// </summary>
	/// <param name="scale">(開始時スケール,終了時スケール)</param>
	void SetScale(const AliceMathF::Vector2& scale);

	/// <summary>
	/// 回転角を設定
	/// </summary>
	/// <param name="rotation">(開始時回転角,終了時回転角)</param>
	void SetRotation(const AliceMathF::Vector2& rotation);

	/// <summary>
	/// 発生間隔を設定
	/// </summary>
	/// <param name="time">発生間隔</param>
	void SetIntervalTime(size_t time);

	/// <summary>
	/// 基準の生存時間を設定
	/// </summary>
	/// <param name="time">生存時間</param>
	void SetLife(UINT time);

	/// <summary>
	/// 一度に発生させる数を設定
	/// </summary>
	/// <param name="index">一度に発生させる数</param>
	void SetIndex(size_t index);

	void SetTex(uint32_t handle);

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新処理
	/// </summary>
	void FireWorkUpdate();

	void BasicUpdate();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(Camera* camera, Material* material = nullptr);

	ParticleEmitter() = default;
	~ParticleEmitter() = default;

private:

	//コピーコンストラクタ・代入演算子削除
	ParticleEmitter& operator=(const ParticleEmitter&) = delete;
	ParticleEmitter(const ParticleEmitter&) = delete;
};

