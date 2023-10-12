#pragma once

#include<Defined.h>

ALICE_SUPPRESS_WARNINGS_BEGIN

#include<stdint.h>
#include<array>
#include<string>

ALICE_SUPPRESS_WARNINGS_END


struct GroupMask
{
	//名前
	std::string name;
	//マスク
	uint32_t mask;

private:

	int8_t PADING[ 4 ];
};

class CollisionFilterConfigTable
{
private:

	//マスク
	//uint32を使って判断するためビットの関係で32が限界
	std::array<GroupMask, 32>groupMasks;

public:

	//デストラクタ・コンストラクタ
	CollisionFilterConfigTable();
	~CollisionFilterConfigTable();

	/// <summary>
	/// マスクの名前を設定
	/// </summary>
	/// <param name="group_">グループID</param>
	/// <param name="name_">名前</param>
	void SetGroupName(uint32_t group_, const char* name_);

	/// <summary>
	/// マスクの名前を取得
	/// </summary>
	/// <param name="group_">グループID</param>
	const char* GetGroupName(uint32_t group_) const;

	/// <summary>
	/// グループ同士の当たり判定を有効化・無効化
	/// </summary>
	/// <param name="groupA_">グループA</param>
	/// <param name="groupB_">グループA</param>
	/// <param name="enable_">trueなら有効falseなら無効</param>
	void EnableCollision(uint32_t groupA_, uint32_t groupB_, bool enable_ = true);

	/// <summary>
	/// グループ同士の当たり判定を有効かどうか
	/// </summary>
	bool IsCollisionEnabled(uint32_t groupA_, uint32_t groupB_) const;

	/// <summary>
	/// ビットマスクを取得
	/// </summary>
	/// <param name="group_">グループID</param>
	uint32_t GetFilterMask(uint32_t group_) const;

	/// <summary>
	/// 名前が付いているグループの数
	/// </summary>
	uint32_t GetNumNamedGroups() const;

	/// <summary>
	/// グループIDを取得
	/// </summary>
	/// <param name="name_">グループ名</param>
	/// <returns>指定された名前のグループが存在しない場合は-1を返す</returns>
	uint32_t GetFilterGroupByName(const std::string& name_) const;

	/// <summary>
	/// 名前のないグループIDを取得
	/// </summary>
	uint32_t FindUnnamedGroup() const;
};

