#pragma once

#include<AliceMathUtility.h>
#include<Sprite.h>
#include<DefaultMaterial.h>
#include<AliceUtility.h>

class Sprite2D : public Sprite
{
private:

	float leftScale = 1.0f;
	float rightScale = 1.0f;
	float topScale = 1.0f;
	float bottomScale = 1.0f;
public:

	Sprite2D();
	~Sprite2D();
	
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="handle">テクスチャハンドル</param>
	virtual void Initialize(uint32_t handle_)override;

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="transform">ワールド変換データ</param>
	/// <param name="blend">ブレンド設定(初期値はアルファブレンド)</param>
	/// <param name="material">ブレンドがCUSTOM時に使う</param>
	virtual void Draw(Transform& transform_,BlendMode blend_ = BlendMode::AX_BLENDMODE_ALPHA, Material* material_ = nullptr)override;

	/// <summary>
	/// アニメーション描画
	/// </summary>
	/// <param name="transform">ワールド変換データ</param>
	/// <param name="radiusX">アニメーション画像の一枚分の横幅</param>
	/// <param name="radiusY">アニメーション画像の一枚分の縦幅</param>
	/// <param name="frame">アニメーションフレーム</param>
	/// <param name="frameDiv">アニメーションフレームの最大値</param>
	/// <param name="blend">ブレンド設定(初期値はアルファブレンド)</param>
	/// <param name="material">ブレンドがCUSTOM時に使う</param>
	virtual void AnimationDraw(Transform& transform_, uint16_t radiusX_, uint16_t radiusY_, float& frame_, float frameDiv_, BlendMode blend_ = BlendMode::AX_BLENDMODE_ALPHA, Material* material_ = nullptr)override;

	float GetLeftSize();
	float GetRightSize();
	float GetTopSize();
	float GetBottomSize();

	void SetLeftScale(float scale_);
	void SetRightScale(float scale_);
	void SetTopScale(float scale_);
	void SetBottomScale(float scale_);

	float GetHeight();
	float GetWidth();

private:

	//コピーコンストラクタ・代入演算子削除
	Sprite2D& operator=(const Sprite2D&) = delete;
	Sprite2D(const Sprite2D&) = delete;

};

/// <summary>
/// 2Dスプライトの生成(ユニーク)
/// </summary>
/// <returns>生成されたポインタ</returns>
/// <param name="handle">テクスチャハンドル</param>
std::unique_ptr<Sprite2D> CreateUniqueSprite2D(uint32_t handle_);

/// <summary>
/// 2Dスプライトの生成(シェアード)
/// </summary>
/// <returns>生成されたポインタ</returns>
/// <param name="handle">テクスチャハンドル</param>
std::shared_ptr<Sprite2D> CreateSharedSprite2D(uint32_t handle_);