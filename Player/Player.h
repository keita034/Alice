#pragma once
#include<GameObject.h>
#include<Input.h>
#include<GameCameraManager.h>
#include<PlayerUI.h>
#include<PlayerAnimation.h>
#include<PlayerWeapon.h>
#include<ActorSituation.h>
#include<DeviceInput.h>
#include<AudioManager.h>
#include<AlicePhysicsSystem.h>

struct PlayerUsData
{
	int32_t situation;
	AliceMathF::Vector3 pos;
};

class Player : public GameObject
{
private:

	AliceInput::IInput* input = nullptr;
	IAudioManager* audioManager = nullptr;
	Camera* camera = nullptr;

	PlayerUsData usData;

	AliceMathF::Vector3 oldTrans;
	float speed = 170.0f;

	AliceMathF::Vector3 direction;
	float scale = 0.002f;

	AliceMathF::Vector3 rowlingWay;
	float rowlingDistance = 300.0f;

	AliceMathF::Vector3 rowlingStartPos;
	float deathSEVolume = 0.04f;

	AliceMathF::Vector3 rowlingEndPos;
	AliceMathF::Vector3 rigidBodyoffset;

	std::unique_ptr<PlayerUI>ui;
	std::unique_ptr<PlayerAnimation>animation;
	std::unique_ptr<PlayerWeapon> weapon;
	std::unique_ptr<DeviceInput> deviceInput;

	const int32_t MAX_HP = 200;
	const int32_t MAX_STAMINA = 2000;
	const int32_t MAX_BULLET = 20;
	const int32_t MAX_HEALING = 20;

	const int32_t MAX_DAMAGE_INTERVAL = 70;
	int32_t hp = 0;
	int32_t stamina = 0;
	int32_t subRunStamina = 2;

	int32_t subAttackStamina = 200;
	int32_t subRowlingStamina = 400;
	int32_t bullet = 0;
	int32_t healing = 0;

	int32_t healingNum = 10;
	int32_t damageInterval = 0;
	uint32_t deathSE = 0;
	uint32_t attackCount = 0;

	bool isStationary = true;
	bool fieldHit = false;
	bool shockwaveHit = false;
	bool attackAdd = false;

public:

	Player() = default;
	~Player() = default;

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(AliceInput::IInput* input_,IAudioManager* audioManager_,AlicePhysics::AlicePhysicsSystem* physicsSystem_);

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update(BaseGameCamera* camera_, GameCameraManager::CameraIndex index_);

	/// <summary>
	/// 描画
	/// </summary>
	void Draw()override;

	/// <summary>
	/// 後始末
	/// </summary>
	void Finalize(AlicePhysics::AlicePhysicsSystem* physicsSystem_)override;

	/// <summary>
	/// 座標取得
	/// </summary>
	const AliceMathF::Vector3& GetPosition()const;

	/// <summary>
	/// 行列計算
	/// </summary>
	void TransUpdate(Camera* camera_)override;

	/// <summary>
	/// UI描画
	/// </summary>
	void UIDraw();

	/// <summary>
	/// リセット
	/// </summary>
	void Reset();

	/// <summary>
	/// 当たった瞬間に呼ばれる
	/// </summary>
	virtual void OnCollisionEnter(AlicePhysics::RigidBodyUserData* BodyData_)override;

	/// <summary>
	/// 当たってる時に呼ばれる
	/// </summary>
	virtual void OnCollisionStay(AlicePhysics::RigidBodyUserData* BodyData_)override;

	/// <summary>
	/// 離れた瞬間に呼ばれる
	/// </summary>
	virtual void OnCollisionExit() override;

	bool IsAttack();

	int32_t GetDamage();

	int32_t GetHp();

	bool IsEnd();

	void AnimationStop();

	void AnimationEndStop();

	void DeathSEChangeVolume(float volume_);

private:

	/// <summary>
	/// 移動
	/// </summary>
	void PMove(BaseGameCamera* camera_);

	void PRowling(BaseGameCamera* camera_);

	void PUIUpdate();

	/// <summary>
	/// 回転
	/// </summary>
	void PRotate();

	/// <summary>
	/// 攻撃
	/// </summary>
	void PAttack();

	void PHealing();

	void Initialize(AlicePhysics::AlicePhysicsSystem* physicsSystem_)override {};
	void Update()override {};
	void Initialize(AlicePhysics::AlicePhysicsSystem* physicsSystem_,uint32_t handle_, const AliceMathF::Vector3& pos_, const AliceMathF::Vector3& rot_ , const AliceMathF::Vector3& scl_, const Transform* parent_)override{};
	//コピーコンストラクタ・代入演算子削除
	Player& operator=(const Player&) = delete;
	Player(const Player&) = delete;

};

