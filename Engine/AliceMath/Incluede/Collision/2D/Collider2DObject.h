#pragma once
#pragma warning(push)
#pragma warning(disable: 4365)
#pragma warning(disable: 4514)
#pragma warning(disable: 4514)
#pragma warning(disable: 4619)
#pragma warning(disable: 4668)
#pragma warning(disable: 4711)
#pragma warning(disable: 5039)

#include<memory>

#pragma warning(pop)

#include<Base2DCollider.h>

class Collider2DObject
{
protected:

	//コライダー
	std::unique_ptr<Base2DCollider>collider;

public:

	/// <summary>
	/// コライダーを取得
	/// </summary>
	Base2DCollider* GetCollider();

	/// <summary>
	/// 当たったら呼ばれる関数
	/// </summary>
	virtual void OnCollision() = 0;
	virtual void CollisionUpdate() = 0;

	virtual ~Collider2DObject() = default;
	Collider2DObject() = default;

protected:

	/// <summary>
	/// コライダーをセット
	/// </summary>
	void SetColl(Base2DCollider* collider_);

private:

	Collider2DObject(const Collider2DObject&) = delete;
	Collider2DObject& operator = (const Collider2DObject&) = delete;

};