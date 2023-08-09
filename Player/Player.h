#pragma once
#include<GameObject.h>
#include<Input.h>
#include<GameCameraManager.h>
#include<PlayerUI.h>
#include<PlayerAnimation.h>
#include<PlayerWeapon.h>
#include<ActorSituation.h>

class Player : public GameObject
{
private:

	AliceInput::IInput* input;

	float speed = 100.0f;
	float scale = 0.35f;
	//float scale = 1.0f;

	AliceMathF::Vector3 oldTrans;
	AliceMathF::Vector3 direction;

	bool isStationary = true;

	const int32_t MAX_HP = 300;
	int32_t hp = 0;

	const int32_t MAX_STAMINA = 100;
	int32_t stamina = 0;
	
	const int32_t MAX_BULLET = 20;
	int32_t bullet = 0;

	const int32_t MAX_HEALING = 20;
	int32_t healing = 0;

	std::unique_ptr<PlayerUI>ui;
	std::unique_ptr<PlayerAnimation>animation;
	std::unique_ptr<PlayerWeapon> weapon;

	Camera* camera = nullptr;

	bool fieldHit = false;

	int32_t situation = 0;

	AliceMathF::Vector3 rowlingWay;
	AliceMathF::Vector3 rowlingStartPos;
	AliceMathF::Vector3 rowlingEndPos;
	float rowlingDistance = 100.0f;

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

	bool IsAttack();

	int32_t GetDamage();

private:

	/// <summary>
	/// 移動
	/// </summary>
	void PMove(BaseGameCamera* camera_);

	void PRowling(BaseGameCamera* camera_);

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

