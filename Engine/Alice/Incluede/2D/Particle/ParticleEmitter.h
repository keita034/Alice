#pragma once
#include<AliceMathUtility.h>
#include<Camera.h>
#include<Material.h>

class IParticleEmitter
{
public:

	/// <summary>
	/// 基準の座標を設定
	/// </summary>
	/// <param name="pos">座標</param>
	virtual void SetPosition(const AliceMathF::Vector3& pos_) = 0;

	/// <summary>
	/// 開始時カラーを設定
	/// </summary>
	/// <param name="color">カラー</param>
	virtual void SetStartColor(const AliceMathF::Vector4& color_) = 0;

	/// <summary>
	/// 終了時カラーを設定
	/// </summary>
	/// <param name="color">カラー</param>
	virtual void SetEndColor(const AliceMathF::Vector4& color_) = 0;

	/// <summary>
	/// 基準の加速度を設定
	/// </summary>
	/// <param name="accel">加速度</param>
	virtual void SetAccel(const AliceMathF::Vector3& accel_) = 0;

	/// <summary>
	/// 基準の速度を設定
	/// </summary>
	/// <param name="velocity">速度</param>
	virtual void SetVelocity(const AliceMathF::Vector3& velocity_) = 0;

	/// <summary>
	/// スケールを設定
	/// </summary>
	/// <param name="scale">(開始時スケール,終了時スケール)</param>
	virtual void SetScale(const AliceMathF::Vector2& scale_) = 0;

	/// <summary>
	/// 回転角を設定
	/// </summary>
	/// <param name="rotation">(開始時回転角,終了時回転角)</param>
	virtual void SetRotation(const AliceMathF::Vector2& rotation_) = 0;

	/// <summary>
	/// 発生間隔を設定
	/// </summary>
	/// <param name="time">発生間隔</param>
	virtual void SetIntervalTime(size_t time_) = 0;

	/// <summary>
	/// 基準の生存時間を設定
	/// </summary>
	/// <param name="time">生存時間</param>
	virtual void SetLife(uint32_t time_) = 0;

	/// <summary>
	/// 一度に発生させる数を設定
	/// </summary>
	/// <param name="index">一度に発生させる数</param>
	virtual void SetIndex(size_t index_) = 0;

	virtual void SetTex(uint32_t handle_) = 0;

	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Initialize() = 0;

	/// <summary>
	/// 更新処理
	/// </summary>
	virtual void FireWorkUpdate() = 0;

	virtual void BasicUpdate() = 0;

	/// <summary>
	/// 描画
	/// </summary>
	virtual void Draw(Camera* camera_, const Material* material_ = nullptr) = 0;

	IParticleEmitter() = default;
	virtual ~IParticleEmitter() = default;

private:

	//コピーコンストラクタ・代入演算子削除
	IParticleEmitter& operator=(const IParticleEmitter&) = delete;
	IParticleEmitter(const IParticleEmitter&) = delete;
};

/// <summary>
/// パーティクルエミッターの生成(ユニーク)
/// </summary>
/// <returns>生成されたポインタ</returns>
std::unique_ptr<IParticleEmitter> CreateUniqueParticleEmitter();

/// <summary>
/// パーティクルエミッターの生成(シェアード)
/// </summary>
/// <returns>生成されたポインタ</returns>
std::shared_ptr<IParticleEmitter> CreateSharedParticleEmitter();