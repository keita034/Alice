#pragma once
#include<GameObject.h>
#include<Input.h>
#include<GameCameraManager.h>
#include<PlayerUI.h>

class Player : public GameObject
{
private:

	AliceInput::IInput* input;

	float speed = 1.0f;
	float scale = 0.35f;

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
private:

	/// <summary>
	/// 移動
	/// </summary>
	void PMove(BaseGameCamera* camera_);

	/// <summary>
	/// 回転
	/// </summary>
	void PRotate();

	void Initialize()override {};
	void Update()override {};
	void Initialize(uint32_t handle_, const AliceMathF::Vector3& pos_, const AliceMathF::Vector3& rot_ , const AliceMathF::Vector3& scl_, const Transform* parent_)override{};
	//コピーコンストラクタ・代入演算子削除
	Player& operator=(const Player&) = delete;
	Player(const Player&) = delete;

};

