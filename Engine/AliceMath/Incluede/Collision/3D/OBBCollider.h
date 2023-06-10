#pragma once
#include"BaseCollider.h"
#include"Structure.h"
class OBBCollider : public BaseCollider
{
protected:
	AliceMathF::Vector3 center = {};//位置
	AliceMathF::Vector3 normaDirect[3];//方向ベクトル
	float length_[3]{};// 各軸方向の長さ
private:
	char PADING[4]{};
public:

	const AliceMathF::Vector3& GetDirect(uint16_t elem);//指定軸番号の方向ベクトルを取得
	float GetLen(uint16_t elem);// 指定軸方向の長さを取得
	const AliceMathF::Vector3& GetCenter();// 位置を取得

	void CreateOBB(const std::vector<PosNormalUv>& vertex,const  AliceMathF::Vector3& trans,const AliceMathF::Vector3& rot,const AliceMathF::Vector3& scal);

	void UpdateOBB(const AliceMathF::Matrix4& worldMat, const AliceMathF::Vector3& rot);
};

