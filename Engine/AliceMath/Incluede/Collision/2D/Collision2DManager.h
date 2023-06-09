﻿#pragma once
#pragma warning(push)
#pragma warning(disable: 4365)
#pragma warning(disable: 4514)
#pragma warning(disable: 4619)
#pragma warning(disable: 4668)
#pragma warning(disable: 5039)

#include<forward_list>

#pragma warning(pop)

#include<Base2DCollider.h>

class Collision2DManager final
{
private:

	//コライダーのリスト
	std::forward_list<Base2DCollider*> colliders;

public:

	/// <summary>
	/// インスタンスを取得
	/// </summary>
	static Collision2DManager* SGetInstance();

	/// <summary>
	/// コライダーを追加
	/// </summary>
	void AddCollider(Base2DCollider* collider_);

	/// <summary>
	/// コライダーを削除
	/// </summary>
	void RemoveCollider(Base2DCollider* collider_);

	/// <summary>
	/// もともとある当たり判定を変える
	/// </summary>
	/// <param name="colliderIndex">変えたい判定の場所</param>
	/// <param name="collider">当たり判定を入れる</param>
	void ChangeCollider(uint32_t colliderIndex_, Base2DCollider* collider_);

	void CheckAllCollisions();

private:

	Collision2DManager() = default;
	~Collision2DManager() = default;

	Collision2DManager(const Collision2DManager&) = delete;
	Collision2DManager& operator = (const Collision2DManager&) = delete;
};

