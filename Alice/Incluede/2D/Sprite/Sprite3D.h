#pragma once
//#DirectXのやつ
#include"ErrorException.h"

//自作.h
#include"AliceMathUtility.h"
#include"WindowsApp.h"
#include"DirectX12Core.h"
#include "Camera.h"
#include"Sprite.h"
#include"DefaultMaterial.h"

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
	virtual void Initialize(uint32_t handle)override;

	/// <summary>
	/// 描画(ビルボードあり)
	/// </summary>
	/// <param name="transform">ワールド変換データ</param>
	/// <param name="camera">カメラ</param>
	/// <param name="blend">ブレンド設定(初期値はアルファブレンド)</param>
	/// <param name="flag">ビルボードの種類(初期値は無効)</param>
	/// <param name="material">ブレンドがCUSTOM時に使う</param>
	virtual void Draw(Transform& transform, Camera* camera, BlendMode blend = BlendMode::AX_BLENDMODE_ALPHA, BillboardFlag flag = NonBillboard, Material* material = nullptr);

	/// <summary>
	/// 描画(ビルボードなし)
	/// </summary>
	/// <param name="transform">ワールド変換データ</param>
	/// <param name="blend">ブレンド設定(初期値はアルファブレンド)</param>
	//// <param name="material">ブレンドがCUSTOM時に使う</param>
	virtual void Draw(Transform& transform, BlendMode blend = BlendMode::AX_BLENDMODE_ALPHA, Material* material = nullptr)override;

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
	virtual void AnimationDraw(Transform& transform, uint16_t radiusX, uint16_t radiusY, float& frame, float frameDiv, BlendMode blend = BlendMode::AX_BLENDMODE_ALPHA, Material* material = nullptr)override;

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
	virtual void AnimationDraw(Transform& transform, uint16_t radiusX, uint16_t radiusY, float& frame, float frameDiv, Camera* camera, BlendMode blend = BlendMode::AX_BLENDMODE_ALPHA, BillboardFlag flag = NonBillboard, Material* material = nullptr);

private:



	//コピーコンストラクタ・代入演算子削除
	Sprite3D& operator=(const Sprite3D&) = delete;
	Sprite3D(const Sprite3D&) = delete;
};

