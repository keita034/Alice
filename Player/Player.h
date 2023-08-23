#pragma once
#include<GameObject.h>
#include<Input.h>
#include<GameCameraManager.h>
#include<PlayerUI.h>
#include<PlayerAnimation.h>
#include<PlayerWeapon.h>
#include<ActorSituation.h>
#include<DeviceInput.h>

class Player : public GameObject
{
private:

	AliceInput::IInput* input = nullptr;
	Camera* camera = nullptr;

	AliceMathF::Vector3 oldTrans;
	AliceMathF::Vector3 direction;

	AliceMathF::Vector3 rowlingWay;
	AliceMathF::Vector3 rowlingStartPos;
	AliceMathF::Vector3 rowlingEndPos;

	std::unique_ptr<PlayerUI>ui;
	std::unique_ptr<PlayerAnimation>animation;
	std::unique_ptr<PlayerWeapon> weapon;
	std::unique_ptr<DeviceInput> deviceInput;

	float speed = 170.0f;
	float scale = 0.35f;
	float rowlingDistance = 100.0f;
	//float scale = 1.0f;

	const int32_t MAX_HP = 3;
	const int32_t MAX_STAMINA = 2000;
	const int32_t MAX_BULLET = 20;
	const int32_t MAX_HEALING = 20;
	const int32_t MAX_DAMAGE_INTERVAL = 20;

	int32_t hp = 0;

	int32_t stamina = 0;
	int32_t subRunStamina = 2;
	int32_t subAttackStamina = 200;
	int32_t subRowlingStamina = 400;

	int32_t bullet = 0;

	int32_t healing = 0;

	int32_t situation = 0;

	int32_t damageInterval = 0;

	bool isStationary = true;
	bool fieldHit = false;

public:

	Player() = default;
	~Player() = default;

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(AliceInput::IInput* input_);

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
	void Finalize()override;

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

	virtual void OnContact(uint32_t attribute_)override;

	virtual void OnTrigger(uint32_t attribute_)override;

	bool IsAttack();

	int32_t GetDamage();

	int32_t GetHp();

	bool IsEnd();

	void AnimationStop();

	void AnimationEndStop();

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

	void PAttack();

	void Initialize()override {};
	void Update()override {};
	void Initialize(uint32_t handle_, const AliceMathF::Vector3& pos_, const AliceMathF::Vector3& rot_ , const AliceMathF::Vector3& scl_, const Transform* parent_)override{};
	//コピーコンストラクタ・代入演算子削除
	Player& operator=(const Player&) = delete;
	Player(const Player&) = delete;

};

