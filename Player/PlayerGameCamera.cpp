#include "PlayerGameCamera.h"

void PlayerGameCamera::Initialize()
{
	camera = std::make_unique<GameCamera>();
	camera->Initialize(UPDATE_PROJMATRIXFUNC_PERSPECTIVE);
	camera->SetFar(2000.0f);
	PCameraPositionUpdate();
}

void PlayerGameCamera::Update()
{
	PCameraPositionUpdate();

}

void PlayerGameCamera::BeginUpdate()
{
	if ( player->GetHp() > 0 )
	{
		PMove();
	}
}

void PlayerGameCamera::Draw()
{
}

void PlayerGameCamera::Finalize()
{

}

void PlayerGameCamera::SetPlayer(Player* player_, AliceInput::IInput* input_)
{
	assert(input_);
	assert(player_);

	player = player_;
	input = input_;
}

PlayerGameCamera::~PlayerGameCamera() = default;

AliceMathF::Vector2 PlayerGameCamera::GetAngle()
{
	return { xAngle,yAngle };
}

void PlayerGameCamera::PMove()
{
	// ZCSXキーでカメラの操作
	if (input->InputStick(ControllerStick::RRIGHT))
	{
		yAngle += ANGLE_ROTATE_SPEED;
		if (yAngle >= 180.0f)
		{
			yAngle -= 360.0f;
		}
	}

	if (input->InputStick(ControllerStick::RLEFT))
	{
		yAngle -= ANGLE_ROTATE_SPEED;
		if (yAngle <= -180.0f)
		{
			yAngle += 360.0f;
		}
	}

	if (input->InputStick(ControllerStick::RUP))
	{
		xAngle += ANGLE_ROTATE_SPEED;
		if (xAngle >= 80.0f)
		{
			xAngle = 80.0f;
		}
	}

	if (input->InputStick(ControllerStick::RDOWN))
	{
		xAngle -= ANGLE_ROTATE_SPEED;
		if (xAngle <= 0.0f)
		{
			xAngle = 0.0f;
		}
	}
}

void PlayerGameCamera::PCameraPositionUpdate()
{
	AliceMathF::Vector3 TempPosition1;
	AliceMathF::Vector3 TempPosition2;
	AliceMathF::Vector3 CameraPosition;
	AliceMathF::Vector3 CameraLookAtPosition;

	CameraLookAtPosition = player->GetPosition();
	CameraLookAtPosition.y += CAMERA_LOOK_AT_HEIGHT;

	// カメラの位置はカメラの水平角度と垂直角度から算出
	float SinParam;
	float CosParam;

	// 最初に垂直角度を反映した位置を算出
	SinParam = AliceMathF::Sin(xAngle / AliceMathF::DEG_TO_RAD);
	CosParam = AliceMathF::Cos(xAngle / AliceMathF::DEG_TO_RAD);
	TempPosition1.x = 0.0f;
	TempPosition1.y = SinParam * CAMERA_LOOK_AT_DISTANCE;
	TempPosition1.z = -CosParam * CAMERA_LOOK_AT_DISTANCE;

	// 次に水平角度を反映した位置を算出
	SinParam = AliceMathF::Sin(yAngle / AliceMathF::DEG_TO_RAD);
	CosParam = AliceMathF::Cos(yAngle / AliceMathF::DEG_TO_RAD);
	TempPosition2.x = CosParam * TempPosition1.x - SinParam * TempPosition1.z;
	TempPosition2.y = TempPosition1.y;
	TempPosition2.z = SinParam * TempPosition1.x + CosParam * TempPosition1.z;

	// 算出した座標に注視点の位置を加算したものがカメラの位置
	CameraPosition = TempPosition2 + CameraLookAtPosition;

	camera->SetEye(CameraPosition);
	camera->SetTarget(CameraLookAtPosition);
}
