#pragma once
#pragma warning(push)
#pragma warning(disable: 4365)
#pragma warning(disable: 4514)
#pragma warning(disable: 4619)
#pragma warning(disable: 4668)
#pragma warning(disable: 4710)
#pragma warning(disable: 5039)

#include<string>
#pragma warning(pop)

#include<AliceMathUtility.h>
#include<CollisionShapeType.h>

class Collider2DObject;

class Base2DCollider
{
protected:

	Collider2DObject* object = nullptr;

	//ポインター(相手)
	Base2DCollider* opponentCollider = nullptr;

	uint32_t collisionAttribute = 0xffffffff;
	//衝突マスク(相手)
	uint32_t CollisionMask = 0xffffffff;

	//名前(自分)
	std::string collsionName;
	//名前(相手)
	std::string opponentCollsionName;

	CollisionShapeType shapeType = SHAPE_UNKNOWN;

	//有効か
	bool isValid = false;
	int8_t PADING[3];

public:
	/// <summary>
	/// 衝突時に呼ばれる関数
	/// </summary>
	virtual void OnCollision();

	/// <summary>
	/// 衝突属性(自分)を取得
	/// </summary>
	uint32_t GetCollisionAttribute();

	/// <summary>
	/// 衝突属性(自分)を取得
	/// </summary>
	void SetCollisionAttribute(uint32_t collisionAttribute_);

	/// <summary>
	/// 衝突属性(相手)を取得
	/// </summary>
	uint32_t GetCollisionMask();

	/// <summary>
	/// 衝突属性(相手)を設定
	/// </summary>
	void SetCollisionMask(uint32_t CollisionMask_);

	/// <summary>
	/// 名前をセット
	/// </summary>
	void SetCollsionName(const std::string& name_);

	/// <summary>
	/// 名前を取得
	/// </summary>
	const std::string& GetCollsionName();

	/// <summary>
	/// 当ったコライダーのポインターをセット
	/// </summary>
	void SetOpponentCollider(Base2DCollider* collider_);

	/// <summary>
	/// 当ったコライダーのポインターを取得
	/// </summary>
	Base2DCollider* GetOpponentCollider();

	/// <summary>
	/// 当たった相手の名前をセット
	/// </summary>
	void SetOpponentCollsionName(const std::string& name_);

	/// <summary>
	/// 当たった相手の名前を取得
	/// </summary>
	const std::string& GetOpponentCollsionName()const;

	/// <summary>
	///	オブジェクトをセット
	/// </summary>
	void SetObject(Collider2DObject* object_);

	/// <summary>
	/// オブジェクトを取得
	/// </summary>
	Collider2DObject* GetColliderObject();

	/// <summary>
	/// 有効にする
	/// </summary>
	void Valid();

	/// <summary>
	/// 無効にする
	/// </summary>
	void Invalid();

	/// <summary>
	/// フラグを取得
	/// </summary>
	bool IsValid();

	/// <summary>
	/// 形状を取得
	/// </summary>
	/// <returns></returns>
	const CollisionShapeType& GetShapeType()const;

	/// <summary>
	/// 更新
	/// </summary>
	virtual void CollisionUpdate();

	virtual ~Base2DCollider() = default;
	Base2DCollider() = default;
};
