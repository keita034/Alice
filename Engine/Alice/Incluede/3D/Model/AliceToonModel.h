#pragma once
#include<AliceModel.h>

class AliceToonModel : public AliceModel
{
private:


public:

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="transform">ワールド変換データ</param>
	/// <param name="material">マテリアル(パイプライン)</param>
	void Draw(Transform& transform, const AliceMotionData* animation = nullptr, float frame = 0.0f, Material* material = nullptr) override;

	void SetRampTexture(const std::string& rampFilePath);

	AliceToonModel() = default;
	~AliceToonModel() = default;

private:

	void ToonModelDraw(Transform& transform);
	void ToonModelAnimationDraw(Transform& transform);

		//コピーコンストラクタ・代入演算子削除
	AliceToonModel& operator=(const AliceToonModel&) = delete;
	AliceToonModel(const AliceToonModel&) = delete;
};

