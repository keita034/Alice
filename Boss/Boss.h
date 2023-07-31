#pragma once
#include<GameObject.h>
#include<Player.h>
#include<FireWorkParticle.h>
#include<AudioManager.h>

class Boss : public GameObject
{
private:

	int32_t situation=0;

	Player* player = nullptr;

	const int32_t MAX_HP = 300;
	int32_t hp = 0;

	std::unique_ptr<FireWorkParticle>fireWorkParticle;

	Camera* camera;

	IAudioManager* audioManager = nullptr;

	int32_t damageSE;

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

	virtual void OnTrigger(uint32_t attribute_)override;

	void SetPlayer(Player* player_);
	void SetAudioManager(IAudioManager* audioManager_);
private:

	void Initialize(uint32_t handle_, const AliceMathF::Vector3& pos_, const AliceMathF::Vector3& rot_, const AliceMathF::Vector3& scl_, const Transform* parent_)override {};
	//コピーコンストラクタ・代入演算子削除
	Boss& operator=(const Boss&) = delete;
	Boss(const Boss&) = delete;
};

