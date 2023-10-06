#pragma once

#include<Defined.h>

ALICE_SUPPRESS_WARNINGS_BEGIN

#include<stdint.h>
#include<string>

ALICE_SUPPRESS_WARNINGS_END

#include<CollisionAttribute.h>

namespace AlicePhysics
{
	class IRigidBody;


	class RigidBodyUserData
	{
	private:

		void* userData = nullptr;
		IRigidBody* rigidBody = nullptr;
		CollisionAttribute attribute;
		CollisionGroup group;
		int8_t PADING[ 6 ];
		std::string name;


	public:

		/// <summary>
		/// 名前を設定
		/// </summary>
		void SetName(const std::string& name_);

		/// <summary>
		/// 属性を設定
		/// </summary>
		void SetAttribute(CollisionAttribute attribute_);

		/// <summary>
		/// グループを設定
		/// </summary>
		void SetGroup(CollisionGroup group_);

		/// <summary>
		/// ユーザーデータを設定
		/// </summary>
		void SetUserData(void* userData_);

		/// <summary>
		/// リジットボディを設定
		/// </summary>
		void SetRigidBody(IRigidBody* rigidBody_);

		/// <summary>
		/// リジットボディを取得
		/// </summary>
		IRigidBody* GetRigidBody();

		/// <summary>
		/// 名前を取得
		/// </summary>
		/// <returns></returns>
		const std::string& GetName()const;

		/// <summary>
		/// 属性を取得
		/// </summary>
		CollisionAttribute GetAttribute()const;

		/// <summary>
		/// グループを取得
		/// </summary>
		CollisionGroup GetGroup()const;

		/// <summary>
		/// ユーザーデータを設定
		/// </summary>
		void* GetUserData();

		/// <summary>
		/// ユーザーデータを設定
		/// </summary>
		void* GetRigidBodyUserData();

		RigidBodyUserData() = default;
		~RigidBodyUserData() = default;
	};
}

