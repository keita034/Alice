#pragma once

#include "CollisionPrimitive.h"
#include"AliceMathUtility.h"

/// <summary>
/// 当たり判定ヘルパークラス
/// </summary>
class Collision
{
public:

#pragma region 点

	/// <summary>
	/// 点と三角形の最近接点を求める
	/// </summary>
	/// <param name="point_">点</param>
	/// <param name="triangle_">三角形</param>
	/// <param name="closest_">最近接点（出力用）</param>
	static void SClosestPtPoint2Triangle(const AliceMathF::Vector4& point_, const TriangleCollider& triangle_, AliceMathF::Vector4* closest_);

#pragma endregion

#pragma region 球

	/// <summary>
	/// 球と球の当たり判定
	/// </summary>
	/// <param name="sphereA_">球A</param>
	/// <param name="sphereB_">球B</param>
	/// <param name="inter_">交点（出力用）</param>
	/// <param name="reject_">排斥ベクトル（出力用）</param>
	/// <returns></returns>
	static bool SCheckSphere2Sphere(const SphereCollider& sphereA_, const SphereCollider& sphereB_, AliceMathF::Vector4* inter_ = nullptr, AliceMathF::Vector4* reject_ = nullptr);

	/// <summary>
	/// 球と平面の当たり判定
	/// </summary>
	/// <param name="sphere_">球</param>
	/// <param name="plane_">平面</param>
	/// <param name="inter_">交点（平面上の最近接点）</param>
	/// <returns>交差しているか否か</returns>
	static bool SCheckSphere2Plane(const SphereCollider& sphere_, const PlaneCollider& plane_, AliceMathF::Vector4* inter_ = nullptr);

	/// <summary>
	/// 球と法線付き三角形の当たり判定
	/// </summary>
	/// <param name="sphere_">球</param>
	/// <param name="triangle_">三角形</param>
	/// <param name="inter_">交点（三角形上の最近接点）</param>
	/// <param name="reject_">排斥ベクトル（出力用）</param>
	/// <returns>交差しているか否か</returns>
	static bool SCheckSphere2Triangle(const SphereCollider& sphere_, const TriangleCollider& triangle_, AliceMathF::Vector4* inter_ = nullptr, AliceMathF::Vector4* reject_ = nullptr);

	/// <summary>
	/// レイと球の当たり判定
	/// </summary>
	/// <param name="sphere_">球</param>
	/// <param name="lay">レイ</param>
	/// <param name="sphere_">距離（出力用）</param>
	/// <param name="inter_">交点（出力用）</param>
	/// <returns>交差しているか否か</returns>
	static bool SCheckSphere2Ray(const SphereCollider& sphere_, const RayCollider& lay, float* distance_ = nullptr, AliceMathF::Vector4* inter_ = nullptr);

	/// <summary>
	/// レイキャストと球の当たり判定
	/// </summary>
	/// <param name="sphere_">球</param>
	/// <param name="lay">レイ</param>
	/// <param name="sphere_">距離（出力用）</param>
	/// <param name="inter_">交点（出力用）</param>
	/// <returns>交差しているか否か</returns>
	static bool SCheckSphere2RayCast(const SphereCollider& sphere_, const RayCastCollider& lay, float* distance_ = nullptr, AliceMathF::Vector4* inter_ = nullptr);


#pragma endregion

#pragma region レイ

	/// <summary>
	/// レイと平面の当たり判定
	/// </summary>
	/// <param name="lay">レイ</param>
	/// <param name="plane_">平面</param>
	/// <param name="inter_">距離（出力用）</param>
	/// <param name="inter_">交点（出力用）</param>
	/// <returns>交差しているか否か</returns>
	static bool SCheckRay2Plane(const RayCollider& lay_, const PlaneCollider& plane_, float* distance_ = nullptr, AliceMathF::Vector4* inter_ = nullptr);

	/// <summary>
	/// レイと法線付き三角形の当たり判定
	/// </summary>
	/// <param name="lay">レイ</param>
	/// <param name="triangle_">三角形</param>
	/// <param name="inter_">距離（出力用）</param>
	/// <param name="inter_">交点（出力用）</param>
	/// <returns>交差しているか否か</returns>
	static bool SCheckRay2Triangle(const RayCollider& lay_, const TriangleCollider& triangle_, float* distance_ = nullptr, AliceMathF::Vector4* inter_ = nullptr);

#pragma endregion

#pragma region OBB

	/// <summary>
	/// OBBとOBBの当たり判定
	/// </summary>
	/// <param name="obb1">OBB</param>
	/// <param name="obb2">OBB</param>
	/// <returns>交差しているか否か</returns>
	static bool SCheckOBB2OBB(const OBBCollider& obb1_, const OBBCollider& obb2_);

#pragma endregion




#pragma region 球
#pragma endregion


#pragma region 2D
	/// <summary>
	/// 点と直線の最短距離
	/// </summary>
	/// <param name="point_">点</param>
	/// <param name="line">無限直線</param>
	/// <param name="mPoint">点から直線に下ろした垂線の端点（出力用）</param>
	/// <param name="t">割合（出力用）</param>
	/// <returns>最短距離</returns>
	static float SPoint2LineDistance(const Point2D& point_, const Line2D& line_, Point2D& mPoint_, float& t_);

	/// <summary>
	/// 点とカプセルの最短距離
	/// </summary>
	/// <param name="point_">点</param>
	/// <param name="capsule">カプセル</param>
	/// <returns>最短距離</returns>
	static float SPoint2CapsuleDistance(const Point2D& point_, const CapsuleCllider2D& capsule_);

#pragma region 矩形

	/// <summary>
	/// 矩形とカプセルの当たり判定
	/// </summary>
	/// <param name="box">矩形</param>
	/// <param name="capsule">カプセル</param>
	static bool SCheckBox2Capsule(const BoxCollider2D& box_, const  CapsuleCllider2D& capsule_);

#pragma endregion

#pragma endregion

};