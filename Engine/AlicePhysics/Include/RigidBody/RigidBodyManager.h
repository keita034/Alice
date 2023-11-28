#pragma once

#include<Defined.h>

ALICE_SUPPRESS_WARNINGS_BEGIN

#include<unordered_map>
#include<memory>

ALICE_SUPPRESS_WARNINGS_END


#include<IRigidBody.h>

namespace AlicePhysics
{
	class RigidBodyManager
	{
	private:

		std::unordered_map<uint32_t,std::unique_ptr<IRigidBody>> rigidBodys;

	public:

		/// <summary>
		/// リジットボディを追加
		/// </summary>
		/// <param name="rigidBody_">リジットボディ</param>
		/// <param name="id_">ID</param>
		void AddRigidBody(std::unique_ptr<IRigidBody>& rigidBody_,uint32_t id_);

		/// <summary>
		/// リジットボディを削除
		/// </summary>
		/// <param name="id_">ID</param>
		void RemoveRigidBody(uint32_t id_);

		/// <summary>
		/// リジットボディを取得
		/// </summary>
		/// <param name="id_">ID</param>
		IRigidBody* GetRigidBody(uint32_t id_);

	};
}

