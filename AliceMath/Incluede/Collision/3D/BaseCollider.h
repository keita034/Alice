﻿#pragma once
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

class ColliderObject;

class BaseCollider
{
protected:

	uint32_t collisionAttribute = 0xffffffff;
	//衝突マスク(相手)
	uint32_t CollisionMask = 0xffffffff;

	//名前(自分)
	std::string collsionName;
	//名前(相手)
	std::string opponentCollsionName;

	ColliderObject* object = nullptr;

	//有効か
	bool isValid = false;

	char PADING[7];

public:
	/// <summary>
	/// 衝突時に呼ばれる関数
	/// </summary>
	virtual void OnCollision();

	/// <summary>
	/// 衝突属性(自分)を取得
	/// </summary>
	const uint32_t GetCollisionAttribute();

	/// <summary>
	/// 衝突属性(自分)を取得
	/// </summary>
	void SetCollisionAttribute(uint32_t collisionAttribute_);

	/// <summary>
	/// 衝突属性(相手)を取得
	/// </summary>
	const uint32_t GetCollisionMask();

	/// <summary>
	/// 衝突属性(相手)を設定
	/// </summary>
	void SetCollisionMask(uint32_t CollisionMask_);

	/// <summary>
	/// 名前をセット
	/// </summary>
	void SetCollsionName(const std::string& name);

	/// <summary>
	/// 名前を取得
	/// </summary>
	const std::string& GetCollsionName();

	/// <summary>
	/// 当たった相手の名前をセット
	/// </summary>
	void SetOpponentCollsionName(const std::string& name);

	/// <summary>
	/// 当たった相手の名前を取得
	/// </summary>
	const std::string& GetOpponentCollsionName();

	/// <summary>
	///	オブジェクトをセット
	/// </summary>
	void SetObject(ColliderObject* obj);

	/// <summary>
	/// オブジェクトを取得
	/// </summary>
	ColliderObject* GetColliderObject();

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



	virtual ~BaseCollider() = default;
	BaseCollider() = default;
};
