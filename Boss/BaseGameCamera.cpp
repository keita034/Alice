#include "BaseGameCamera.h"

void BaseGameCamera::CameraUpdate()
{
	transform.matWorld = AliceMathF::MakeWorldMatrix4(transform, quaternion);

	camera->SetEye( AliceMathF::GetWorldPosition(transform));
	//ワールド前方ベクトル
	AliceMathF::Vector3 forward(0, 0, 1);
	//レールカメラの回転を反映
	forward = AliceMathF::Vec3Mat4Mul(forward, transform.matWorld);
	//視点から前方に適当な距離進んだ位置が注視点
	camera->SetTarget(camera->GetEye() + forward);
	//ワールド上方ベクトル
	AliceMathF::Vector3 up(0, 1, 0);
	//レールカメラの回転を反映(レールカメラの上方ベクトル)
	camera->SetUp(AliceMathF::Vec3Mat4Mul(up, transform.matWorld));
}

GameCamera* BaseGameCamera::GetCamera()
{
	return camera.get();
}

Camera* BaseGameCamera::GetGameCamera()
{
	return camera.get();
}

BaseGameCamera::~BaseGameCamera() = default;
