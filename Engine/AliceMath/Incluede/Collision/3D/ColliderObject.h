#pragma once
#pragma warning(push)
#pragma warning(disable: 4365)
#pragma warning(disable: 4514)
#pragma warning(disable: 4619)
#pragma warning(disable: 4668)
#pragma warning(disable: 4711)
#pragma warning(disable: 5039)

#include<memory>

#pragma warning(pop)

#include<BaseCollider.h>


class ColliderObject
{
protected:

	//コライダー
	std::unique_ptr<BaseCollider>collider;

public:

	/// <summary>
	/// コライダーを取得
	/// </summary>
	BaseCollider* GetCollider();

	/// <summary>
	/// 当たったら呼ばれる関数
	/// </summary>
	virtual void OnCollision() = 0;

	virtual ~ColliderObject() = default;
	ColliderObject() = default;

protected:

	/// <summary>
	/// コライダーをセット
	/// </summary>
	void SetColl(BaseCollider* collider_);

private:

	ColliderObject(const ColliderObject&) = delete;
	void operator = (const ColliderObject&) = delete;
};

