#pragma once
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
	void SetColl(BaseCollider* coll);

private:

	ColliderObject(const ColliderObject&) = delete;
	void operator = (const ColliderObject&) = delete;
};

