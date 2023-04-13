#include "CinemaCamera.h"
#include"WindowsApp.h"

void CinemaCamera::Initialize()
{
	//アスペクト比を計算する
	aspect =
		static_cast<float>(WindowsApp::GetWindowsSize().width) /
		static_cast<float>(WindowsApp::GetWindowsSize().height);

	if (near_ == 0.0f)
	{
		near_ = 0.1f;
	}
	if (far_ == 0.0f)
	{
		far_ = 1000.0f;
	}
	if (fovAngleY == 0.0f)
	{
		fovAngleY = fovAngleY = 2 * AliceMathF::Atan(AliceMathF::GetDiagonal(cameraSensorSize.width, cameraSensorSize.height) / (2 * focalLength));
	}

	//透視投影行列の計算
	AliceMathF::MakePerspectiveL(fovAngleY, aspect, near_, far_, projectionMatrix);

	updateProjMatrix = false;

	//ビュー行列の算出
	AliceMathF::MakeLookL(eye, target, up, viewMatrix);
	//ビュー行列の逆行列を計算
	viewMatrixInv = AliceMathF::MakeInverse(&viewMatrix);
	//ビュープロジェクション行列の作成
	viewProjectionMatrix = viewMatrix * projectionMatrix;

	//カメラの前ベクトル取得
	forward = { viewMatrixInv.m[2][0], viewMatrixInv.m[2][1], viewMatrixInv.m[2][2] };
	forward.Normal();

	//注視点と視点の距離取得
	AliceMathF::Vector3 toPos;
	toPos = eye - target;
	tgtToPosLen = toPos.Length();

	updateViewMatrix = false;
}

void CinemaCamera::Update()
{
	if (updatefovAngleY)
	{
		fovAngleY = 2 * AliceMathF::Atan(AliceMathF::GetDiagonal(cameraSensorSize.width, cameraSensorSize.height) / (2 * focalLength));

		updatefovAngleY = false;
	}

	if (updateProjMatrix)
	{
		AliceMathF::MakePerspectiveL(fovAngleY, aspect, near_, far_, projectionMatrix);

		updateProjMatrix = false;
	}

	if (updateViewMatrix)
	{
		//ビュー行列の算出
		AliceMathF::MakeLookL(eye, target, up, viewMatrix);
		//ビュープロジェクション行列の作成
		viewProjectionMatrix = viewMatrix * projectionMatrix;
		//ビュー行列の逆行列を計算
		viewMatrixInv = AliceMathF::MakeInverse(&viewMatrix);

		forward = { viewMatrixInv.m[2][0], viewMatrixInv.m[2][1], viewMatrixInv.m[2][2] };

		AliceMathF::Vector3 toPos;
		toPos = eye - target;
		tgtToPosLen = toPos.Length();

		updateViewMatrix = false;
	}
}

void CinemaCamera::Move(const AliceMathF::Vector3& move)
{
	eye += move;
	target += move;
	updateViewMatrix = true;
}

void CinemaCamera::MoveTarget(const AliceMathF::Vector3& move)
{
	target += move;
	updateViewMatrix = true;
}

void CinemaCamera::MovePosition(const AliceMathF::Vector3& move)
{
	eye += move;
	updateViewMatrix = true;
}

void CinemaCamera::SetAspect(float aspect_)
{
	aspect = aspect_;
	updateViewMatrix = true;
}

void CinemaCamera::SetFar(float fFar)
{
	far_ = fFar;
	updateProjMatrix = true;
}

void CinemaCamera::SetNear(float fNear)
{
	near_ = fNear;
	updateProjMatrix = true;
}

void CinemaCamera::SetFocalLength(float length)
{
	focalLength = length;
	updatefovAngleY = true;
	updateProjMatrix = true;

}

void CinemaCamera::SetAperture(float value)
{
	aperture = value;
}

void CinemaCamera::SetSensorSize(SensorSize size)
{
	sensorSize = size;
	switch (size)
	{
	case SensorSize::MEDIUM_FORMAT:
		cameraSensorSize = { 43.8f,32.8f };
		break;
	case SensorSize::FF:
		cameraSensorSize = { 36.0f,24.0f };
		break;
	case SensorSize::APS_C:
		cameraSensorSize = { 23.5f,15.6f };
		break;
	case SensorSize::M_FOUR_THIRDS:
		cameraSensorSize = { 17.3f,13.0f };
		break;
	case SensorSize::ONE_INCH:
		cameraSensorSize = { 13.2f,8.8f };
		break;
	case SensorSize::ONE_OVER_ONE_POINT_SEVEN_INCH:
		cameraSensorSize = { 7.5f,5.6f };
		break;
	case SensorSize::ONE_THIRD_INCH:
		cameraSensorSize = { 4.8f,3.6f };
		break;
	default:
		break;
	}
	updatefovAngleY = true;
}

void CinemaCamera::SetShootingDistance(float distance)
{
	shootingDistance = distance;
}

void CinemaCamera::SetEye(const AliceMathF::Vector3& pos)
{
	eye = pos;
	updateViewMatrix = true;

}

void CinemaCamera::SetTarget(const AliceMathF::Vector3& pos)
{
	target = pos;
	updateViewMatrix = true;
}

void CinemaCamera::SetUp(const AliceMathF::Vector3& vec)
{
	up = vec;
	updateViewMatrix = true;
}

const AliceMathF::Matrix4& CinemaCamera::GetViewMatrix()
{
	//更新
	Update();
	return viewMatrix;
}

const AliceMathF::Matrix4& CinemaCamera::GetViewMatrixInv()
{
	//更新
	Update();
	return viewMatrixInv;
}

const AliceMathF::Matrix4& CinemaCamera::GetProjectionMatrix()
{
	//更新
	Update();
	return projectionMatrix;
}

const AliceMathF::Matrix4& CinemaCamera::GetViewProjectionMatrix()
{
	//更新
	Update();
	return viewProjectionMatrix;
}

const AliceMathF::Matrix4& CinemaCamera::GetCameraRotation()
{
	//更新
	Update();
	return cameraRotation;
}

float CinemaCamera::GetFar() const
{
	return far_;
}

float CinemaCamera::GetNear() const
{
	return near_;
}

float CinemaCamera::GetFovAngleY() const
{
	return fovAngleY;
}

float CinemaCamera::GetTargetToPositionLength() const
{
	return tgtToPosLen;
}

float CinemaCamera::GetFocalLength() const
{
	return focalLength;
}

float CinemaCamera::GetAperture() const
{
	return aperture;
}

float CinemaCamera::GetDistance() const
{
	return shootingDistance;
}

const AliceMathF::Vector3& CinemaCamera::GetForward() const
{
	return forward;
}

float CinemaCamera::GetAspect() const
{
	return aspect;
}

const AliceMathF::Vector3& CinemaCamera::GetEye()
{
	return eye;
}

const AliceMathF::Vector3& CinemaCamera::GetUp()
{
	return up;
}

const AliceMathF::Vector3& CinemaCamera::GetTarget()
{
	return target;
}
