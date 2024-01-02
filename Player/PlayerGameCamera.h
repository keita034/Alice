#pragma once
#include<BaseGameCamera.h>
#include<Player.h>

class PlayerGameCamera : public BaseGameCamera
{
private:

	Player* player = nullptr;
	AliceInput::IInput* input = nullptr;

	AliceMathF::Vector3 direction;

	AliceMathF::Vector3 targetPos;

	float yAngle;
	float xAngle;

	const float ANGLE_ROTATE_SPEED = 0.0005f;
	const float CAMERA_LOOK_AT_HEIGHT= 50.0f;
	const float CAMERA_LOOK_AT_DISTANCE = 120.0f;

public:

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize()override;

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update()override;

	/// <summary>
	/// 更新処理
	/// </summary>
	void BeginUpdate()override;

	/// <summary>
	/// 描画
	/// </summary>
	void Draw() override;

	/// <summary>
	/// 後始末
	/// </summary>
	void Finalize() override;

	void SetPlayer(Player* player_, AliceInput::IInput* input_);

	PlayerGameCamera() = default;
	~PlayerGameCamera();

	AliceMathF::Vector2 GetAngle();

private:

	void PMove();

	void PCameraPositionUpdate();
};

