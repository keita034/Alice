#include<CinemaCamera.h>
#include<WindowsApp.h>

void CinemaCamera::Initialize()
{
	//アスペクト比を計算する
	aspect =
		static_cast<float>(sWindowsApp->GetWindowSize().width) /
		static_cast<float>(sWindowsApp->GetWindowSize().height);

	if (nearClip == 0.0f)
	{
		nearClip = 0.1f;
	}
	if (farClip == 0.0f)
	{
		farClip = 1000.0f;
	}
	if (fovAngleY == 0.0f)
	{
		fovAngleY = fovAngleY = 2 * AliceMathF::Atan(AliceMathF::GetDiagonal(cameraSensorSize.width, cameraSensorSize.height) / (2 * focallength));
	}

	//透視投影行列の計算
	AliceMathF::MakePerspectiveL(fovAngleY, aspect, nearClip, farClip, projectionMatrix);

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
	AliceMathF::Vector3 lTgtToPosLen;
	lTgtToPosLen = eye - target;
	tgtToPosLen = lTgtToPosLen.length_();

	updateViewMatrix = false;
}

void CinemaCamera::Update()
{
	if (updatefovAngleY)
	{
		fovAngleY = 2 * AliceMathF::Atan(AliceMathF::GetDiagonal(cameraSensorSize.width, cameraSensorSize.height) / (2 * focallength));

		updatefovAngleY = false;
	}

	if (updateProjMatrix)
	{
		AliceMathF::MakePerspectiveL(fovAngleY, aspect, nearClip, farClip, projectionMatrix);

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

		AliceMathF::Vector3 lTgtToPosLen;
		lTgtToPosLen = eye - target;
		tgtToPosLen = lTgtToPosLen.length_();

		updateViewMatrix = false;
	}
}

void CinemaCamera::Move(const AliceMathF::Vector3& move_)
{
	eye += move_;
	target += move_;
	updateViewMatrix = true;
}

void CinemaCamera::MoveTarget(const AliceMathF::Vector3& move_)
{
	target += move_;
	updateViewMatrix = true;
}

void CinemaCamera::MovePosition(const AliceMathF::Vector3& move_)
{
	eye += move_;
	updateViewMatrix = true;
}

void CinemaCamera::SetAspect(float aspect_)
{
	aspect = aspect_;
	updateViewMatrix = true;
}

void CinemaCamera::SetFar(float far_)
{
	farClip = far_;
	updateProjMatrix = true;
}

void CinemaCamera::SetNear(float near_)
{
	nearClip = near_;
	updateProjMatrix = true;
}

void CinemaCamera::SetFocallength_(float length_)
{
	focallength = length_;
	updatefovAngleY = true;
	updateProjMatrix = true;

}

void CinemaCamera::SetAperture(float value_)
{
	aperture = value_;
}

void CinemaCamera::SetSensorSize(SensorSize size_)
{
	sensorSize = size_;
	switch (size_)
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

void CinemaCamera::SetShootingDistance(float distance_)
{
	shootingDistance = distance_;
}

void CinemaCamera::SetEye(const AliceMathF::Vector3& pos_)
{
	eye = pos_;
	updateViewMatrix = true;

}

void CinemaCamera::SetTarget(const AliceMathF::Vector3& pos_)
{
	target = pos_;
	updateViewMatrix = true;
}

void CinemaCamera::SetUp(const AliceMathF::Vector3& vec_)
{
	up = vec_;
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
	return farClip;
}

float CinemaCamera::GetNear() const
{
	return nearClip;
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
	return focallength;
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

const AliceMathF::Vector3& CinemaCamera::GetEye()const
{
	return eye;
}

const AliceMathF::Vector3& CinemaCamera::GetUp()const
{
	return up;
}

const AliceMathF::Vector3& CinemaCamera::GetTarget()const
{
	return target;
}
