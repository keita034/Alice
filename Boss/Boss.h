#pragma once
#include<GameObject.h>
#include<Player.h>
#include<FireWorkParticle.h>
#include<AudioManager.h>

#include<BossActionManager.h>
#include<BossAnimation.h>
#include<BossHand.h>
#include<BossUI.h>

class Boss : public GameObject
{
private:

	Player* player = nullptr;
	Camera* camera = nullptr;
	IAudioManager* audioManager = nullptr;

	std::unique_ptr<FireWorkParticle>fireWorkParticle;
	std::unique_ptr<BossActionManager> actionManager;
	std::unique_ptr<BossAnimation> animation;
	std::unique_ptr<BossUI>bossUI;

	std::array< std::unique_ptr<BossHand>,2> hands;

	AliceMathF::Vector3 direction = { 0,0,1 };
	AliceMathF::Vector3 oldTrans;

	const int32_t MAX_HP = 3;
	
	int32_t damageSE;
	int32_t situation = 0;
	int32_t hp = 0;


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

	/// <summary>
	/// リセット
	/// </summary>
	void Reset();

	void TransUpdate(Camera* camera_)override;

	virtual void OnTrigger(uint32_t attribute_)override;

	void SetPlayer(Player* player_);
	void SetAudioManager(IAudioManager* audioManager_);

	int32_t GetHp();

	bool IsEnd();

	void AnimationStop();

	void AnimationEndStop();

private:

	void Initialize(uint32_t handle_, const AliceMathF::Vector3& pos_, const AliceMathF::Vector3& rot_, const AliceMathF::Vector3& scl_, const Transform* parent_)override {};
	//コピーコンストラクタ・代入演算子削除
	Boss& operator=(const Boss&) = delete;
	Boss(const Boss&) = delete;
};

