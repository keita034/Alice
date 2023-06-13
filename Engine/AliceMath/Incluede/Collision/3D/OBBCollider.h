#pragma once
#include"BaseCollider.h"
#include"Structure.h"
class OBBCollider : public BaseCollider
{
protected:
	//位置
	AliceMathF::Vector3 center = {};
	//方向ベクトル
	AliceMathF::Vector3 normaDirect[3];
	// 各軸方向の長さ
	float length_[3]{};
private:
	char PADING[4]{};
public:

	/// <summary>
	/// 指定軸番号の方向ベクトルを取得
	/// </summary>
	const AliceMathF::Vector3& GetDirect(uint16_t index_)const;

	/// <summary>
	/// 指定軸方向の長さを取得
	/// </summary>
	float GetLen(uint16_t index_)const;

	/// <summary>
	/// 中心座標を取得
	/// </summary>
	/// <returns></returns>
	const AliceMathF::Vector3& GetCenter()const;

	/// <summary>
	/// OBBを生成
	/// </summary>
	/// <param name="vertexs_">頂点配列</param>
	/// <param name="translation_">中心座標</param>
	/// <param name="rotation_">回転角</param>
	/// <param name="scale_">scale</param>
	void CreateOBB(const std::vector<PosNormalUv>& vertexs_, const  AliceMathF::Vector3& translation_, const AliceMathF::Vector3& rotation_, const AliceMathF::Vector3& scale_);

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="worldMatrix_">ワールド行列</param>
	/// <param name="rotation_">回転角</param>
	void UpdateOBB(const AliceMathF::Matrix4& worldMatrix_, const AliceMathF::Vector3& rotation_);
};

