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
	virtual void Initialize(uint32_t handle)override;

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="transform">ワールド変換データ</param>
	/// <param name="blend">ブレンド設定(初期値はアルファブレンド)</param>
	/// <param name="material">ブレンドがCUSTOM時に使う</param>
	virtual void Draw(Transform& transform,BlendMode blend = BlendMode::AX_BLENDMODE_ALPHA, Material* material = nullptr)override;

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
	virtual void AnimationDraw(Transform& transform, uint16_t radiusX, uint16_t radiusY, float& frame, float frameDiv, BlendMode blend = BlendMode::AX_BLENDMODE_ALPHA, Material* material = nullptr)override;

	float GetLeftSize();
	float GetRightSize();
	float GetTopSize();
	float GetBottomSize();

	void SetLeftScale(float scl);
	void SetRightScale(float scl);
	void SetTopScale(float scl);
	void SetBottomScale(float scl);

	float GetHeight();
	float GetWidth();

private:


	//コピーコンストラクタ・代入演算子削除
	Sprite2D& operator=(const Sprite2D&) = delete;
	Sprite2D(const Sprite2D&) = delete;

};