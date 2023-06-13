#pragma warning(push)
#pragma warning(disable: 4365)
#pragma warning(disable: 4514)
#pragma warning(disable: 4619)
#pragma warning(disable: 4668)
#pragma warning(disable: 4711)
#pragma warning(disable: 5039)

#include<cmath>
#include<memory>

#pragma warning(pop)

#include "Collision.h"

#pragma region 点

void Collision::SClosestPtPoint2Triangle(const AliceMathF::Vector4& point_, const TriangleCollider& triangle_, AliceMathF::Vector4* closest_)
{
	// point_がp0の外側の頂点領域の中にあるかどうかチェック
	AliceMathF::Vector4 lP0_P1 = triangle_.GetP0() - triangle_.GetP1();
	AliceMathF::Vector4 lP0_P2 = triangle_.GetP2() - triangle_.GetP0();
	AliceMathF::Vector4 lP0_PT = point_ - triangle_.GetP0();

	float lD1 = lP0_PT.Dot(lP0_P1);
	float lD2 = lP0_PT.Dot(lP0_P2);

	if (lD1 <= 0.0f && lD2 <= 0.0f)
	{
		// p0が最近傍
		*closest_ = triangle_.GetP0();
		return;
	}

	// pointがp1の外側の頂点領域の中にあるかどうかチェック
	AliceMathF::Vector4 lP1_PT = point_ - triangle_.GetP1();

	float lD3 = lP1_PT.Dot(lP0_P1);
	float lD4 = lP1_PT.Dot(lP0_P2);

	if (lD3 >= 0.0f && lD4 <= lD3)
	{
		// p1が最近傍
		*closest_ = triangle_.GetP1();
		return;
	}

	// point_がlP0_P1の辺領域の中にあるかどうかチェックし、あればpoint_のlP0_P1上に対する射影を返す
	float lVC = lD1 * lD4 - lD3 * lD2;
	if (lVC <= 0.0f && lD1 >= 0.0f && lD3 <= 0.0f)
	{
		float lV = lD1 / (lD1 - lD3);
		*closest_ = triangle_.GetP0() + lV * lP0_P1;
		return;
	}

	// point_がp2の外側の頂点領域の中にあるかどうかチェック
	AliceMathF::Vector4 lP2_PT = point_ - triangle_.GetP2();

	float lD5 = lP2_PT.Dot(lP0_P1);
	float lD6 = lP2_PT.Dot(lP0_P2);
	if (lD6 >= 0.0f && lD5 <= lD6)
	{
		*closest_ = triangle_.GetP2();
		return;
	}

	// point_がlP0_P2の辺領域の中にあるかどうかチェックし、あればpoint_のlP0_P2上に対する射影を返す
	float lVB = lD5 * lD2 - lD1 * lD6;
	if (lVB <= 0.0f && lD2 >= 0.0f && lD6 <= 0.0f)
	{
		float lW = lD2 / (lD2 - lD6);
		*closest_ = triangle_.GetP0() + lW * lP0_P2;
		return;
	}

	// point_がp1_p2の辺領域の中にあるかどうかチェックし、あればpoint_のp1_p2上に対する射影を返す
	float va = lD3 * lD6 - lD5 * lD4;
	if (va <= 0.0f && (lD4 - lD3) >= 0.0f && (lD5 - lD6) >= 0.0f)
	{
		float lW = (lD4 - lD3) / ((lD4 - lD3) + (lD5 - lD6));
		*closest_ = triangle_.GetP1() + lW * (triangle_.GetP2() - triangle_.GetP1());
		return;
	}

	// point_は面領域の中にある。closest_を重心座標を用いて計算する
	float lDenom = 1.0f / (va + lVB + lVC);
	float lV = lVB * lDenom;
	float lW = lVC * lDenom;
	*closest_ = triangle_.GetP0() + lP0_P1 * lV + lP0_P2 * lW;
}

#pragma endregion

#pragma region 球

bool Collision::SCheckSphere2Sphere(const SphereCollider& sphereA_, const SphereCollider& sphereB_, AliceMathF::Vector4* inter_, AliceMathF::Vector4* reject_)
{
	// 中心点の距離の２乗 <= 半径の和の２乗　なら交差
	AliceMathF::Vector4 lTmp;
	lTmp = sphereA_.GetCenter() - sphereB_.GetCenter();
	float lDist = lTmp.lengthSquared();
	float lRadius2 = sphereA_.GetRadius() + sphereB_.GetRadius();
	lRadius2 *= lRadius2;

	if (lDist <= lRadius2)
	{
		if (inter_)
		{
			// Aの半径が0の時座標はBの中心　Bの半径が0の時座標はAの中心　となるよう補完
			float lT = sphereB_.GetRadius() / (sphereA_.GetRadius() + sphereB_.GetRadius());
			*inter_ = AliceMathF::Vector4Lerp(sphereA_.GetCenter(), sphereB_.GetCenter(), lT);
		}
		// 押し出すベクトルを計算
		if (reject_)
		{
			float lRejectLen = sphereA_.GetRadius() + sphereB_.GetRadius() - std::sqrtf(lDist);
			lTmp = sphereA_.GetCenter() - sphereB_.GetCenter();
			*reject_ = lTmp.Normal();
			*reject_ *= lRejectLen;
		}

		return true;
	}

	return false;
}

bool Collision::SCheckSphere2Plane(const SphereCollider& sphere_, const PlaneCollider& plane_, AliceMathF::Vector4* inter_)
{
	// 座標系の原点から球の中心座標への距離から
	// 平面の原点距離を減算することで、平面と球の中心との距離が出る
	float lDist = sphere_.GetCenter().Dot(plane_.GetNormal()) - plane_.GetDistance();

	// 距離の絶対値が半径より大きければ当たっていない
	if (fabsf(lDist) > sphere_.GetRadius())
	{
		return false;
	}

	// 擬似交点を計算
	if (inter_)
	{
		// 平面上の再接近点を、疑似交点とする
		*inter_ = -lDist * plane_.GetNormal() + sphere_.GetCenter() + sphere_.GetCenter();
	}

	return true;
}

bool Collision::SCheckSphere2Triangle(const SphereCollider& sphere_, const TriangleCollider& triangle_, AliceMathF::Vector4* inter_, AliceMathF::Vector4* reject_)
{
	AliceMathF::Vector4 lPoint;
	// 球の中心に対する最近接点である三角形上にある点lPointを見つける
	AliceMathF::Vector4 lCenter = sphere_.GetCenter();
	SClosestPtPoint2Triangle(lCenter, triangle_, &lPoint);

	// 点lPointと球の中心の差分ベクトル
	AliceMathF::Vector4 lV = lPoint - sphere_.GetCenter();

	// 距離の二乗を求める
	//（同じベクトル同士の内積は三平方の定理のルート内部の式と一致する）
	float lDistanceSquare = lV.Dot(lV);

	// 球と三角形の距離が半径以下なら当たっていない
	if (lDistanceSquare > sphere_.GetRadius() * sphere_.GetRadius())
	{
		return false;
	}

	// 擬似交点を計算
	if (inter_)
	{
		// 三角形上の最近接点lPointを疑似交点とする
		*inter_ = lPoint;
	}

	// 押し出すベクトルを計算
	if (reject_)
	{
		float lDS = sphere_.GetCenter().Dot(triangle_.GetNormal());
		float lDT = triangle_.GetP0().Dot(triangle_.GetNormal());
		float lRejectLen = lDT - lDS + sphere_.GetRadius();
		*reject_ = triangle_.GetNormal() * lRejectLen;
	}

	return true;
}

bool Collision::SCheckSphere2Ray(const SphereCollider& sphere_, const RayCollider& lay_, float* distance_, AliceMathF::Vector4* inter_)
{
	AliceMathF::Vector4 lM = lay_.GetStart() - sphere_.GetCenter();
	float lB = lM.Dot(lay_.GetDir());
	float lC = lM.Dot(lM) - sphere_.GetRadius() * sphere_.GetRadius();

	// lay_の始点がsphere_の外側にあり(lC > 0)、lay_がsphere_から離れていく方向を
	// 差している場合(lB > 0)、当たらない
	if (lC > 0.0f && lB > 0.0f)
	{
		return false;
	}

	float lDiscr = lB * lB - lC;

	// 負の判別式はレイが球を外れていることに一致
	if (lDiscr < 0.0f)
	{
		return false;
	}

	// レイは球と交差している。
	// 交差する最小の値lTを計算
	float lT = -lB - sqrtf(lDiscr);

	// lTが負である場合、レイは球の内側から開始しているのでlTをゼロにクランプ
	if (lT < 0) lT = 0.0f;

	if (distance_)
	{
		*distance_ = lT;
	}

	if (inter_)
	{
		*inter_ = lay_.GetStart() + lT * lay_.GetDir();
	}

	return true;
}

bool Collision::SCheckSphere2RayCast(const SphereCollider& sphere_, const RayCastCollider& lay_, float* distance_, AliceMathF::Vector4* inter_)
{
	float lDis = 0.0f;

	if (!SCheckSphere2Ray(sphere_, lay_, &lDis, inter_))
	{
		return false;
	}

	if (lDis > lay_.GetRange())
	{
		return false;
	}

	if (distance_)
	{
		*distance_ = lDis;
	}

	return true;
}

#pragma endregion

#pragma region レイ

bool Collision::SCheckRay2Plane(const RayCollider& lay_, const PlaneCollider& plane_, float* distance_, AliceMathF::Vector4* inter_)
{
	const float lEPSILON = 1.0e-5f;	// 誤差吸収用の微小な値

	float lD1 = plane_.GetNormal().Dot(lay_.GetDir());
	// 裏面には当たらない
	if (lD1 > -lEPSILON)
	{
		return false;
	}

	float lD2 = plane_.GetNormal().Dot(lay_.GetStart());
	float lT = (plane_.GetDistance() - lD2) / lD1;

	if (lT < 0)
	{
		return false;
	}

	// 距離を書き込む
	if (distance_)
	{
		*distance_ = lT;
	}

	// 交点を計算
	if (inter_)
	{
		*inter_ = lay_.GetStart() + lT * lay_.GetDir();
	}

	return true;
}

bool Collision::SCheckRay2Triangle(const RayCollider& lay_, const TriangleCollider& triangle_, float* distance_, AliceMathF::Vector4* inter_)
{
	// 三角形が乗っている平面を算出
	std::unique_ptr<PlaneCollider>lPlane;
	lPlane = std::make_unique<PlaneCollider>();

	AliceMathF::Vector4 lInterPlane;
	AliceMathF::Vector4 lNormal = triangle_.GetNormal();
	lPlane->SetNormal(lNormal);
	lPlane->SetDistance(triangle_.GetNormal().Dot(triangle_.GetP0()));
	// レイと平面が当たっていなければ、当たっていない	
	if (!SCheckRay2Plane(lay_, *lPlane, distance_, &lInterPlane))
	{
		return false;
	}
	// レイと平面が当たっていたので、距離と交点が書き込まれた

	// レイと平面の交点が三角形の内側にあるか判定
	const float lEPSILON = 1.0e-5f;	// 誤差吸収用の微小な値
	AliceMathF::Vector4 lM;
	// 辺lP0_P1について
	AliceMathF::Vector4 lPT_P0 = triangle_.GetP0() - lInterPlane;
	AliceMathF::Vector4 lP0_P1 = triangle_.GetP1() - triangle_.GetP0();
	lM = lPT_P0.Cross(lP0_P1);
	// 辺の外側
	if (lM.Dot(triangle_.GetNormal()) < -lEPSILON)
	{
		return false;
	}

	// 辺p1_p2について
	AliceMathF::Vector4 lPT_P1 = triangle_.GetP1() - lInterPlane;
	AliceMathF::Vector4 lP1_P2 = triangle_.GetP2() - triangle_.GetP1();
	lM = lPT_P1.Cross(lP1_P2);
	// 辺の外側
	if (lM.Dot(triangle_.GetNormal()) < -lEPSILON)
	{
		return false;
	}

	// 辺p2_p0について
	AliceMathF::Vector4 lPT_P2 = triangle_.GetP2() - lInterPlane;
	AliceMathF::Vector4 lP2_P0 = triangle_.GetP0() - triangle_.GetP2();
	lM = lPT_P2.Cross(lP2_P0);
	// 辺の外側
	if (lM.Dot(triangle_.GetNormal()) < -lEPSILON)
	{
		return false;
	}

	if (inter_)
	{
		*inter_ = lInterPlane;
	}

	// 内側なので、当たっている
	return true;
}

#pragma endregion

#pragma region OBB

bool Collision::SCheckOBB2OBB(const OBBCollider& obb1_, const OBBCollider& obb2_)
{
	//各方向ベクトルの確保
	//(N***:標準化方向ベクトル)
	AliceMathF::Vector3 lNAe1 = obb1_.GetDirect(0), lAe1 = lNAe1 * obb1_.GetLen(0);
	AliceMathF::Vector3 lNAe2 = obb1_.GetDirect(1), lAe2 = lNAe2 * obb1_.GetLen(1);
	AliceMathF::Vector3 lNAe3 = obb1_.GetDirect(2), lAe3 = lNAe3 * obb1_.GetLen(2);
	AliceMathF::Vector3 lNBe1 = obb2_.GetDirect(0), lBe1 = lNBe1 * obb2_.GetLen(0);
	AliceMathF::Vector3 lNBe2 = obb2_.GetDirect(1), lBe2 = lNBe2 * obb2_.GetLen(1);
	AliceMathF::Vector3 lNBe3 = obb2_.GetDirect(2), lBe3 = lNBe3 * obb2_.GetLen(2);
	AliceMathF::Vector3 lInterval = obb1_.GetCenter() - obb2_.GetCenter();

	//分離軸:lAe1
	float lRA = lAe1.length_();
	float lRB = AliceMathF::LenSegOnSeparateAxis(&lNAe1, &lBe1, &lBe2, &lBe3);
	float lL = AliceMathF::Abs(lInterval.Dot(lNAe1));
	if (lL > lRA + lRB)
	{
		//衝突していない
		return false;
	}

	//分離軸:lAe2
	lRA = lAe2.length_();
	lRB = AliceMathF::LenSegOnSeparateAxis(&lNAe2, &lBe1, &lBe2, &lBe3);
	lL = AliceMathF::Abs(lInterval.Dot(lNAe2));
	if (lL > lRA + lRB)
	{
		//衝突していない
		return false;
	}

	//分離軸:lAe3
	lRA = lAe3.length_();
	lRB = AliceMathF::LenSegOnSeparateAxis(&lNAe3, &lBe1, &lBe2, &lBe3);
	lL = AliceMathF::Abs(lInterval.Dot(lNAe3));
	if (lL > lRA + lRB)
	{
		//衝突していない
		return false;
	}

	//分離軸:lBe1
	lRA = AliceMathF::LenSegOnSeparateAxis(&lNBe1, &lAe1, &lAe2, &lAe3);
	lRB = lBe1.length_();
	lL = AliceMathF::Abs(lInterval.Dot(lNBe1));
	if (lL > lRA + lRB)
	{
		//衝突していない
		return false;
	}

	//分離軸:lBe2
	lRA = AliceMathF::LenSegOnSeparateAxis(&lNBe2, &lAe1, &lAe2, &lAe3);
	lRB = lBe2.length_();
	lL = AliceMathF::Abs(lInterval.Dot(lNBe2));
	if (lL > lRA + lRB)
	{
		//衝突していない
		return false;
	}

	//分離軸:lBe3
	lRA = AliceMathF::LenSegOnSeparateAxis(&lNBe3, &lAe1, &lAe2, &lAe3);
	lRB = lBe3.length_();
	lL = AliceMathF::Abs(lInterval.Dot(lNBe3));
	if (lL > lRA + lRB)
	{
		//衝突していない
		return false;
	}

	//分離軸:C11
	AliceMathF::Vector3 lCross;
	lCross = lNAe1.Cross(lNBe1);
	lRA = AliceMathF::LenSegOnSeparateAxis(&lCross, &lAe2, &lAe3);
	lRB = AliceMathF::LenSegOnSeparateAxis(&lCross, &lBe2, &lBe3);
	lL = AliceMathF::Abs(lInterval.Dot(lCross));
	if (lL > lRA + lRB)
	{
		//衝突していない
		return false;
	}

	//分離軸:C12
	lCross = lNAe1.Cross(lNBe2);
	lRA = AliceMathF::LenSegOnSeparateAxis(&lCross, &lAe2, &lAe3);
	lRB = AliceMathF::LenSegOnSeparateAxis(&lCross, &lBe1, &lBe3);
	lL = AliceMathF::Abs(lInterval.Dot(lCross));
	if (lL > lRA + lRB)
	{
		//衝突していない
		return false;
	}

	//分離軸:C13
	lCross = lNAe1.Cross(lNBe3);
	lRA = AliceMathF::LenSegOnSeparateAxis(&lCross, &lAe2, &lAe3);
	lRB = AliceMathF::LenSegOnSeparateAxis(&lCross, &lBe1, &lBe2);
	lL = AliceMathF::Abs(lInterval.Dot(lCross));
	if (lL > lRA + lRB)
	{
		//衝突していない
		return false;
	}

	//分離軸:C21
	lCross = lNAe2.Cross(lNBe1);
	lRA = AliceMathF::LenSegOnSeparateAxis(&lCross, &lAe1, &lAe3);
	lRB = AliceMathF::LenSegOnSeparateAxis(&lCross, &lBe2, &lBe3);
	lL = AliceMathF::Abs(lInterval.Dot(lCross));
	if (lL > lRA + lRB)
	{
		//衝突していない
		return false;
	}

	//分離軸:C22
	lCross = lNAe2.Cross(lNBe2);
	lRA = AliceMathF::LenSegOnSeparateAxis(&lCross, &lAe1, &lAe3);
	lRB = AliceMathF::LenSegOnSeparateAxis(&lCross, &lBe1, &lBe3);
	lL = AliceMathF::Abs(lInterval.Dot(lCross));
	if (lL > lRA + lRB)
	{
		//衝突していない
		return false;
	}

	//分離軸:C23
	lCross = lNAe2.Cross(lNBe3);
	lRA = AliceMathF::LenSegOnSeparateAxis(&lCross, &lAe1, &lAe3);
	lRB = AliceMathF::LenSegOnSeparateAxis(&lCross, &lBe1, &lBe2);
	lL = AliceMathF::Abs(lInterval.Dot(lCross));
	if (lL > lRA + lRB)
	{
		//衝突していない
		return false;
	}

	//分離軸:C31
	lCross = lNAe3.Cross(lNBe1);
	lRA = AliceMathF::LenSegOnSeparateAxis(&lCross, &lAe1, &lAe2);
	lRB = AliceMathF::LenSegOnSeparateAxis(&lCross, &lBe2, &lBe3);
	lL = AliceMathF::Abs(lInterval.Dot(lCross));
	if (lL > lRA + lRB)
	{
		//衝突していない
		return false;
	}

	//分離軸:C32
	lCross = lNAe3.Cross(lNBe2);
	lRA = AliceMathF::LenSegOnSeparateAxis(&lCross, &lAe1, &lAe2);
	lRB = AliceMathF::LenSegOnSeparateAxis(&lCross, &lBe1, &lBe3);
	lL = AliceMathF::Abs(lInterval.Dot(lCross));
	if (lL > lRA + lRB)
	{
		//衝突していない
		return false;
	}

	//分離軸:C33
	lCross = lNAe3.Cross(lNBe3);
	lRA = AliceMathF::LenSegOnSeparateAxis(&lCross, &lAe1, &lAe2);
	lRB = AliceMathF::LenSegOnSeparateAxis(&lCross, &lBe1, &lBe2);
	lL = AliceMathF::Abs(lInterval.Dot(lCross));
	if (lL > lRA + lRB)
	{
		//衝突していない
		return false;
	}

	//分離平面が存在しないので衝突している
	return true;
}

#pragma endregion

#pragma region 2D

#pragma region 矩形

float Collision::SPoint2LineDistance(const Point2D& point_, const Line2D& line_, Point2D& mPoint_, float& t_)
{
	t_ = 0.0f;
	AliceMathF::Vector2 lVec = line_.GetAxis()* line_.Getlength_();
	float lDvv = lVec.Dot(lVec);

	if (lDvv > 0.0f)
	{
		t_ = lVec.Dot(point_ - line_.GetStart()) / lDvv;
	}

	mPoint_ = line_.GetStart() + lVec * t_;
	return (point_ - mPoint_).length_();
}


float Collision::SPoint2CapsuleDistance(const Point2D& point_, const CapsuleCllider2D& capsule_)
{
	float lDistance = 0.0;

	Segment2D lSegment = capsule_.GetSegment();

	float lX0 = point_.x, lY0 = point_.y;
	float lX1 = lSegment.GetStart().x, lY1 = lSegment.GetStart().y;
	float lX2 = lSegment.GetEnd().x, lY2 = lSegment.GetEnd().y;

	float lA = lX2 - lX1;
	float lB = lY2 - lY1;
	float lA2 = lA * lA;
	float lB2 = lB * lB;
	float lR2 = lA2 + lB2;
	float lTt = -(lA * (lX1 - lX0) + lB * (lY1 - lY0));

	if (lTt < 0)
	{
		lDistance = sqrtf((lX1 - lX0) * (lX1 - lX0) + (lY1 - lY0) * (lY1 - lY0));
		return lDistance;
	}
	else if (lTt > lR2)
	{
		lDistance = sqrtf((lX2 - lX0) * (lX2 - lX0) + (lY2 - lY0) * (lY2 - lY0));
		return lDistance;
	}

	float lF1 = lA * (lY1 - lY0) - lB * (lX1 - lX0);
	lDistance = sqrtf((lF1 * lF1) / lR2);

	return lDistance;
}

bool Collision::SCheckBox2Capsule(const BoxCollider2D& box_, const CapsuleCllider2D& capsule_)
{
	AliceMathF::Vector2 lLeftTop = box_.GetLeftTop();
	AliceMathF::Vector2 lRightBottom = box_.GetRightBottom();

	//左上
	float lDistance = SPoint2CapsuleDistance(lLeftTop, capsule_);

	if (lDistance < capsule_.GetRadius())
	{
		return true;
	}

	//左下
	lDistance = SPoint2CapsuleDistance({ lLeftTop.x, lRightBottom.y }, capsule_);

	if (lDistance < capsule_.GetRadius() )
	{
		return true;
	}

	//右上
	lDistance = SPoint2CapsuleDistance({ lRightBottom.x, lLeftTop.y }, capsule_);

	if (lDistance < capsule_.GetRadius())
	{
		return true;
	}

	//右下
	lDistance = SPoint2CapsuleDistance(lRightBottom, capsule_);

	if (lDistance < capsule_.GetRadius())
	{
		return true;
	}

	return false;
}

#pragma endregion

#pragma endregion