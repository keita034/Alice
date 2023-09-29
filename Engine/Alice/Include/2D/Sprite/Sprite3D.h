#pragma once

//自作.h
#include<AliceMathUtility.h>
#include<WindowsApp.h>
#include<Camera.h>
#include<Sprite.h>
#include<DefaultMaterial.h>

class Sprite3D : public Sprite
{
private:

public:

	Sprite3D();
	~Sprite3D();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="handle">テクスチャハンドル</param>
	virtual void Initialize(uint32_t handle_)override;

	/// <summary>
	/// 描画(ビルボードあり)
	/// </summary>
	/// <param name="transform">ワールド変換データ</param>
	/// <param name="camera">カメラ</param>
	/// <param name="blend">ブレンド設定(初期値はアルファブレンド)</param>
	/// <param name="flag">ビルボードの種類(初期値は無効)</param>
	/// <param name="material">ブレンドがCUSTOM時に使う</param>
	virtual void Draw( Transform& transform_,  Camera* camera_, BlendMode blend_ = BlendMode::AX_BLENDMODE_ALPHA, BillboardFlag flag_ = NON_BILLBOARD, const Material* material_ = nullptr);

	/// <summary>
	/// 描画(ビルボードなし)
	/// </summary>
	/// <param name="transform">ワールド変換データ</param>
	/// <param name="blend">ブレンド設定(初期値はアルファブレンド)</param>
	//// <param name="material">ブレンドがCUSTOM時に使う</param>
	virtual void Draw(Transform& transform_, BlendMode blend_ = BlendMode::AX_BLENDMODE_ALPHA, const Material* material_ = nullptr)override;

	/// <summary>
	/// アニメーション描画(ビルボードなし)
	/// </summary>
	/// <param name="transform">ワールド変換データ</param>
	/// <param name="radiusX">アニメーション画像の一枚分の横幅</param>
	/// <param name="radiusY">アニメーション画像の一枚分の縦幅</param>
	/// <param name="frame">アニメーションフレーム</param>
	/// <param name="frameDiv">アニメーションフレームの最大値</param>
	/// <param name="blend">ブレンド設定(初期値はアルファブレンド)</param>
	/// <param name="material">ブレンドがCUSTOM時に使う</param>
	virtual void AnimationDraw(Transform& transform_, uint16_t radiusX_, uint16_t radiusY_, float& frame_, float frameDiv_, BlendMode blend_ = BlendMode::AX_BLENDMODE_ALPHA, const Material* material_ = nullptr)override;

	/// <summary>
	/// アニメーション描画(ビルボードあり)
	/// </summary>
	/// <param name="transform">ワールド変換データ</param>
	/// <param name="radiusX">アニメーション画像の一枚分の横幅</param>
	/// <param name="radiusY">アニメーション画像の一枚分の縦幅</param>
	/// <param name="frame">アニメーションフレーム</param>
	/// <param name="frameDiv">アニメーションフレームの最大値</param>
	//// <param name="camera">カメラ</param>
	/// <param name="blend">ブレンド設定(初期値はアルファブレンド)</param>
	/// <param name="material">ブレンドがCUSTOM時に使う</param>
	virtual void AnimationDraw(Transform& transform_, uint16_t radiusX_, uint16_t radiusY_, float& frame_, float frameDiv_, Camera* camera_, BlendMode blend_ = BlendMode::AX_BLENDMODE_ALPHA, BillboardFlag flag_ = NON_BILLBOARD, const Material* material_ = nullptr);

private:

	//コピーコンストラクタ・代入演算子削除
	Sprite3D& operator=(const Sprite3D&) = delete;
	Sprite3D(const Sprite3D&) = delete;
};

/// <summary>
/// 3Dスプライトの生成(ユニーク)
/// </summary>
/// <param name="handle">テクスチャハンドル</param>
/// <returns>生成されたポインタ</returns>
std::unique_ptr<Sprite3D> CreateUniqueSprite3D(uint32_t handle_);

/// <summary>
/// 3Dスプライトの生成(シェアード)
/// </summary>
/// <param name="handle">テクスチャハンドル</param>
/// <returns>生成されたポインタ</returns>
std::shared_ptr<Sprite3D> CreateSharedSprite3D(uint32_t handle_);