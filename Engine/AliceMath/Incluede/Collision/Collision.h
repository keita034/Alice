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
	/// <param name="point">点</param>
	/// <param name="triangle">三角形</param>
	/// <param name="closest">最近接点（出力用）</param>
	static void ClosestPtPoint2Triangle(AliceMathF::Vector4& point, TriangleCollider& triangle, AliceMathF::Vector4* closest);

#pragma endregion

#pragma region 球

	/// <summary>
	/// 球と球の当たり判定
	/// </summary>
	/// <param name="sphereA">球A</param>
	/// <param name="sphereB">球B</param>
	/// <param name="inter">交点（出力用）</param>
	/// <param name="reject">排斥ベクトル（出力用）</param>
	/// <returns></returns>
	static bool CheckSphere2Sphere(SphereCollider& sphereA, SphereCollider& sphereB, AliceMathF::Vector4* inter = nullptr, AliceMathF::Vector4* reject = nullptr);

	/// <summary>
	/// 球と平面の当たり判定
	/// </summary>
	/// <param name="sphere">球</param>
	/// <param name="plane">平面</param>
	/// <param name="inter">交点（平面上の最近接点）</param>
	/// <returns>交差しているか否か</returns>
	static bool CheckSphere2Plane(SphereCollider& sphere, PlaneCollider& plane, AliceMathF::Vector4* inter = nullptr);

	/// <summary>
	/// 球と法線付き三角形の当たり判定
	/// </summary>
	/// <param name="sphere">球</param>
	/// <param name="triangle">三角形</param>
	/// <param name="inter">交点（三角形上の最近接点）</param>
	/// <param name="reject">排斥ベクトル（出力用）</param>
	/// <returns>交差しているか否か</returns>
	static bool CheckSphere2Triangle(SphereCollider& sphere, TriangleCollider& triangle, AliceMathF::Vector4* inter = nullptr, AliceMathF::Vector4* reject = nullptr);

	/// <summary>
	/// レイと球の当たり判定
	/// </summary>
	/// <param name="sphere">球</param>
	/// <param name="lay">レイ</param>
	/// <param name="sphere">距離（出力用）</param>
	/// <param name="inter">交点（出力用）</param>
	/// <returns>交差しているか否か</returns>
	static bool CheckSphere2Ray(SphereCollider& sphere,RayCollider& lay, float* distance = nullptr, AliceMathF::Vector4* inter = nullptr);

	/// <summary>
	/// レイキャストと球の当たり判定
	/// </summary>
	/// <param name="sphere">球</param>
	/// <param name="lay">レイ</param>
	/// <param name="sphere">距離（出力用）</param>
	/// <param name="inter">交点（出力用）</param>
	/// <returns>交差しているか否か</returns>
	static bool CheckSphere2RayCast(SphereCollider& sphere,RayCastCollider& lay, float* distance = nullptr, AliceMathF::Vector4* inter = nullptr);


#pragma endregion

#pragma region レイ

	/// <summary>
	/// レイと平面の当たり判定
	/// </summary>
	/// <param name="lay">レイ</param>
	/// <param name="plane">平面</param>
	/// <param name="inter">距離（出力用）</param>
	/// <param name="inter">交点（出力用）</param>
	/// <returns>交差しているか否か</returns>
	static bool CheckRay2Plane( RayCollider& lay,  PlaneCollider& plane, float* distance = nullptr, AliceMathF::Vector4* inter = nullptr);

	/// <summary>
	/// レイと法線付き三角形の当たり判定
	/// </summary>
	/// <param name="lay">レイ</param>
	/// <param name="triangle">三角形</param>
	/// <param name="inter">距離（出力用）</param>
	/// <param name="inter">交点（出力用）</param>
	/// <returns>交差しているか否か</returns>
	static bool CheckRay2Triangle( RayCollider& lay,  TriangleCollider& triangle, float* distance = nullptr, AliceMathF::Vector4* inter = nullptr);

#pragma endregion

#pragma region OBB

	/// <summary>
	/// OBBとOBBの当たり判定
	/// </summary>
	/// <param name="obb1">OBB</param>
	/// <param name="obb2">OBB</param>
	/// <returns>交差しているか否か</returns>
	static bool CheckOBB2OBB(OBBCollider& obb1, OBBCollider& obb2);

#pragma endregion




#pragma region 球
#pragma endregion


#pragma region 2D
	/// <summary>
	/// 点と直線の最短距離
	/// </summary>
	/// <param name="point">点</param>
	/// <param name="line">無限直線</param>
	/// <param name="mPoint">点から直線に下ろした垂線の端点（出力用）</param>
	/// <param name="t">割合（出力用）</param>
	/// <returns>最短距離</returns>
	static float Point2LineDistance(const Point2D& point, Line2D& line, Point2D& mPoint, float& t);

	/// <summary>
	/// 点とカプセルの最短距離
	/// </summary>
	/// <param name="point">点</param>
	/// <param name="capsule">カプセル</param>
	/// <param name="mPoint">点から直線に下ろした垂線の端点（出力用）</param>
	/// <param name="t">割合（出力用）</param>
	/// <returns>最短距離</returns>
	static float Point2CapsuleDistance(const Point2D& point, CapsuleCllider2D& capsule);

#pragma region 矩形

	/// <summary>
	/// 矩形とカプセルの当たり判定
	/// </summary>
	/// <param name="box">矩形</param>
	/// <param name="capsule">カプセル</param>
	/// <returns></returns>
	static bool CheckBox2Capsule(BoxCollider2D& box, CapsuleCllider2D& capsule);

#pragma endregion

#pragma endregion

};