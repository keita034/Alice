#pragma once

#include<AliceMathUtility.h>
#include<Camera.h>
#include<Material.h>

class IIcosahedronParticle
{
public:

	IIcosahedronParticle() = default;
	virtual ~IIcosahedronParticle() = default;

	//初期化
	virtual void Initialize()=0;

	///<summary>
	///更新
	///</summary>
	virtual void Update() = 0;

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
		uint32_t life_,
		const AliceMathF::Vector3& position_,
		const AliceMathF::Vector3& velocity_,
		const AliceMathF::Vector3& accel_,
		const AliceMathF::Vector2& scale_,
		const AliceMathF::Vector2& rotation_,
		const AliceMathF::Vector4& sColor_,
		const AliceMathF::Vector4& eColor_) = 0;

	///<summary>
	///ビルボード描画
	///</summary>
	virtual void Draw(Camera* camera_, Material* material_ = nullptr) = 0;

private:

	//コピーコンストラクタ・代入演算子削除
	IIcosahedronParticle& operator=(const IIcosahedronParticle&) = delete;
	IIcosahedronParticle(const IIcosahedronParticle&) = delete;
};

/// <summary>
/// 八面体パーティクルの生成(ユニーク)
/// </summary>
/// <returns>生成されたポインタ</returns>
std::unique_ptr<IIcosahedronParticle> CreateUniqueIcosahedronParticle();

/// <summary>
/// 八面体パーティクルの生成(シェアード)
/// </summary>
/// <returns>生成されたポインタ</returns>
std::shared_ptr<IIcosahedronParticle> CreateSharedIcosahedronParticle();