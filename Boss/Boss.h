#pragma once
#include<GameObject.h>
#include<Player.h>
#include<FireWorkParticle.h>
#include<AudioManager.h>
#include<GPUParticleEmitter.h>

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
	GPUParticleEmitter* particleEmitter = nullptr;

	std::unique_ptr<FireWorkParticle>fireWorkParticle;
	std::unique_ptr<BossActionManager> actionManager;
	std::unique_ptr<BossAnimation> animation;
	std::unique_ptr<BossUI>bossUI;

	std::array< std::unique_ptr<BossHand>,2> hands;

	AliceMathF::Vector3 direction = { 0,0,1 };
	AliceMathF::Vector3 oldTrans;
	AliceMathF::Vector3 rigidBodyoffset;

	const int32_t MAX_HP = 7;

	uint32_t damageSE = 0;
	uint32_t deathSE = 0;
	float deathSEVolume = 0.04f;

	int32_t situation = 0;
	int32_t hp = 0;
public:

	Boss() = default;
	~Boss() = default;

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(AlicePhysics::AlicePhysicsSystem* physicsSystem_)override;

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update()override;

	/// <summary>
	/// 描画
	/// </summary>
	void Draw()override;

	void UIDraw();

	/// <summary>
	/// 後始末
	/// </summary>
	void Finalize(AlicePhysics::AlicePhysicsSystem* physicsSystem_)override;

	/// <summary>
	/// リセット
	/// </summary>
	void Reset();

	void TransUpdate(Camera* camera_)override;

	/// <summary>
	/// 当たった瞬間に呼ばれる
	/// </summary>
	void OnCollisionEnter(AlicePhysics::RigidBodyUserData* BodyData_)override;

   /// <summary>
   /// 当たってる時に呼ばれる
   /// </summary>
	void OnCollisionStay(AlicePhysics::RigidBodyUserData* BodyData_)override;

	void SetPlayer(Player* player_);
	void SetAudioManager(IAudioManager* audioManager_);
	void SetFireGPUParticle(GPUParticleEmitter* particleEmitter_);

	int32_t GetHp();

	bool IsEnd();

	void AnimationStop();

	void AnimationEndStop();

	void DeathSEChangeVolume(float volume_);

	void OnCollisionExit() override;

private:

	void Initialize(AlicePhysics::AlicePhysicsSystem* physicsSystem_,uint32_t handle_,const AliceMathF::Vector3& pos_,const AliceMathF::Vector3& rot_,const AliceMathF::Vector3& scl_,const Transform* parent_)override {};
	//コピーコンストラクタ・代入演算子削除
	Boss& operator=(const Boss&) = delete;
	Boss(const Boss&) = delete;
};

