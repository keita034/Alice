#pragma once

/// <summary>
/// 衝突判定用関連の型定義
/// </summary>
enum CollisionShapeType
{
	// 未設定
	SHAPE_UNKNOWN = -1,

	// 球
	COLLISIONSHAPE_SPHERE,

	//AABB
	COLLISIONSHAPE_AABB,

	//カプセル
	COLLISIONSHAPE_CAPSULE,

	//OBB
	COLLISIONSHAPE_OBB,

	//無限平面
	COLLISIONSHAPE_PLANE,

	//レイ(距離あり)
	COLLISIONSHAPE_RAYCAST,

	//無限レイ
	COLLISIONSHAPE_RAY,

	//三角形
	COLLISIONSHAPE_TRIANGLE,

	//メッシュ
	COLLISIONSHAPE_MESH,

	//矩形
	COLLISIONSHAPE_BOX

};