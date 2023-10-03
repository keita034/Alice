#pragma once

#include<Defined.h>

ALICE_SUPPRESS_WARNINGS_BEGIN
#include<string>

ALICE_SUPPRESS_WARNINGS_END

#include<AliceMathF.h>
#include<CollisionAttribute.h>
#include<PhysicsRigidBodyType.h>
#include<IShape.h>

namespace AlicePhysics
{
	class IRigidBody;

	// 物理ボディのモーションタイプ
	enum class MotionType : uint8_t
	{
		STATIC,
		KINEMATIC,
		DYNAMIC,
	};

	struct IRigidBodyCreationSettings
	{
		//親
		IRigidBody* parent = nullptr;
		//ユーザーデータ
		void* userData = nullptr;
		//形
		IShape* shape;
		//名前
		std::string name;
		// モーションタイプ
		MotionType motionType;
		//位置
		AliceMathF::Vector3 position;
		//回転
		AliceMathF::Quaternion rotation;
		//摩擦係数
		float friction = 0.2f;
		//反発係数
		float restitution = 0.0f;
		//質量
		float mass = 0.0f;
		//線形減衰(移動の減衰加減?)
		float linearDamping = 0.05f;
		// 角度減衰(回転の減衰加減?)
		float angularDamping = 0.05f;
		//重力の影響度
		float gravityFactor = 1.0f;
		//最大直線速度
		float maxLinearVelocity = 500.0f;
		//最大角速度
		float maxAngularVelocity = 0.25f * AliceMathF::AX_PI * 60.0f;
		//リジットボディの種類
		PhysicsRigidBodyType type;
		//オブジェクトタイプ
		CollisionAttribute collisionAttribute = CollisionAttribute::DEFAULT;
		//オブジェクトグループ
		CollisionGroup collisionGroup = CollisionGroup::DEFAULT;
		//トリガー形状
		bool trigger = false;
		//最初から動くか
		bool isActive = true;
		//一定時間動かなっかたら眠りにつくか
		bool allowSleeping = true;
		//リニアキャストを使用した衝突検出
		bool linearCast = false;
	};
}

