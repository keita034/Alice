﻿#pragma once
#include<AliceModel.h>

class AliceToonModel : public AliceModel
{
private:

	Material* outLineMaterialData = nullptr;

public:

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="transform">ワールド変換データ</param>
	/// <param name="material">マテリアル(パイプライン)</param>
	void Draw(const Transform& transform_, const AliceMotionData* animation_ = nullptr, const Material* material_ = nullptr) override;

	/// <summary>
	/// 描画
	/// </summary>
	virtual void Draw(const Transform& transform_,AliceBlendTree* blendTree_,const Material* material_ = nullptr)override;

	/// <summary>
	/// 描画
	/// </summary>
	virtual void Draw(const Transform& transform_)override;

	/// <summary>
	/// 描画
	/// </summary>
	virtual void Draw(const Transform& transform_,const Material* material_)override;

	/// <summary>
	/// ゼルダの伝説風描画
	/// </summary>
	/// <param name="transform">ワールド変換データ</param>
	/// <param name="material">マテリアル(パイプライン)</param>
	void ZeldaDraw(const Transform& transform_, const AliceMotionData* animation_ = nullptr, const Material* material_ = nullptr);

	void SetRampTexture(const std::string& rampFilePath_);

	AliceToonModel() = default;
	~AliceToonModel() = default;

private:

	void PToonModelDraw(const Transform& transform_);
	void PToonModelAnimationDraw(const Transform& transform_);

	//コピーコンストラクタ・代入演算子削除
	AliceToonModel& operator=(const AliceToonModel&) = delete;
	AliceToonModel(const AliceToonModel&) = delete;
};

