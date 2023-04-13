#pragma once
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
	static Collision2DManager* GetInstance();

	/// <summary>
	/// コライダーを追加
	/// </summary>
	void AddCollider(Base2DCollider* collider);

	/// <summary>
	/// コライダーを削除
	/// </summary>
	void RemoveCollider(Base2DCollider* collide);

	/// <summary>
	/// もともとある当たり判定を変える
	/// </summary>
	/// <param name="colliderIndex">変えたい判定の場所</param>
	/// <param name="collider">当たり判定を入れる</param>
	void ChangeCollider(UINT colliderIndex, Base2DCollider* collider);

	void CheckAllCollisions();

private:

	Collision2DManager() = default;
	~Collision2DManager() = default;

	Collision2DManager(const Collision2DManager&) = delete;
	Collision2DManager& operator = (const Collision2DManager&) = delete;
};

