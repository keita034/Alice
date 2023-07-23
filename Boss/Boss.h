#pragma once
#include<GameObject.h>

class Boss : public GameObject
{
private:

public:

	Boss() = default;
	~Boss() = default;

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize()override;

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update()override;

	/// <summary>
	/// 描画
	/// </summary>
	void Draw()override;

	/// <summary>
	/// 後始末
	/// </summary>
	void Finalize()override;

	void TransUpdate(Camera* camera_)override;

private:

	void Initialize(uint32_t handle_, const AliceMathF::Vector3& pos_, const AliceMathF::Vector3& rot_, const AliceMathF::Vector3& scl_, const Transform* parent_)override {};
	//コピーコンストラクタ・代入演算子削除
	Boss& operator=(const Boss&) = delete;
	Boss(const Boss&) = delete;
};

