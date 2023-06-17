#include "Camera.h"

IWindowsApp* Camera::sWindowsApp = nullptr;

void GameCamera::Initialize(UpdateProjMatrixFunc matFunc_)
{
	//アスペクト比を計算する
	aspect =
		static_cast<float>(sWindowsApp->GetWindowSize().width) /
		static_cast<float>(sWindowsApp->GetWindowSize().height);

	matrixFunc = matFunc_;

	if (matFunc_ == UpdateProjMatrixFunc_Ortho)//平行投影
	{
		if (nearClip == 0.0f)
		{
			nearClip = 0.1f;
		}
		if (farClip == 0.0f)
		{
			farClip = 1000.0f;
		}
		right = static_cast<float>(sWindowsApp->GetWindowSize().width);
		bottom = static_cast<float>(sWindowsApp->GetWindowSize().height);

		//平行投影の計算
		AliceMathF::MakeOrthogonalL(right, bottom, nearClip, farClip, projectionMatrix);

	}
	else//透視投影
	{
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
			fovAngleY = AliceMathF::AX_PI / 4;
		}

		//透視投影行列の計算
		AliceMathF::MakePerspectiveL(fovAngleY, aspect, nearClip, farClip, projectionMatrix);

	}

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

void GameCamera::Update()
{
	if (updateProjMatrix)
	{
		if (matrixFunc == UpdateProjMatrixFunc_Ortho)
		{

			right = static_cast<float>(sWindowsApp->GetWindowSize().width);
			bottom = static_cast<float>(sWindowsApp->GetWindowSize().height);
			AliceMathF::MakeOrthogonalL(right, bottom, nearClip, farClip, projectionMatrix);

		}
		else
		{
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
				fovAngleY = AliceMathF::AX_PI / 4;
			}

			AliceMathF::MakePerspectiveL(fovAngleY, aspect, nearClip, farClip, projectionMatrix);
		}
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

void GameCamera::Move(const AliceMathF::Vector3& move_)
{
	eye += move_;
	target += move_;
	updateViewMatrix = true;
}

void GameCamera::MoveTarget(const AliceMathF::Vector3& move_)
{
	target += move_;
	updateViewMatrix = true;
}

void GameCamera::MovePosition(const AliceMathF::Vector3& move_)
{
	eye += move_;
	updateViewMatrix = true;
}

#pragma region セッター

void GameCamera::SetAspect(float aspect_)
{
	aspect = aspect_;
	updateViewMatrix = true;
}

void GameCamera::SetFar(float far_)
{
	farClip = far_;
	updateProjMatrix = true;
}

void GameCamera::SetNear(float near_)
{
	nearClip = near_;
	updateProjMatrix = true;
}

void GameCamera::SetFovAngleY(float fovAngle_)
{
	fovAngleY = fovAngle_;
	updateProjMatrix = true;
}

void GameCamera::SetUpdateProjMatrixFunc(UpdateProjMatrixFunc func_)
{
	matrixFunc = func_;
	updateProjMatrix = true;
}

void GameCamera::SetLeft(float left_)
{
	left = left_;
	updateProjMatrix = true;
}
void GameCamera::SetRight(float right_)
{
	right = right_;
	updateProjMatrix = true;
}

void GameCamera::SetTop(float top_)
{
	top = top_;
	updateProjMatrix = true;
}

void GameCamera::SetBottom(float bottom_)
{
	bottom = bottom_;
	updateProjMatrix = true;
}

void GameCamera::SetEye(const AliceMathF::Vector3& pos_)
{
	updateViewMatrix = true;
	eye = pos_;
}

void GameCamera::SetTarget(const AliceMathF::Vector3& pos_)
{
	updateViewMatrix = true;
	target = pos_;
}

void GameCamera::SetUp(const AliceMathF::Vector3& vec_)
{
	updateViewMatrix = true;
	up = vec_;
}

#pragma endregion

#pragma region ゲッター

const AliceMathF::Matrix4& GameCamera::GetViewMatrix()
{
	//更新
	Update();
	return viewMatrix;
}

const AliceMathF::Matrix4& GameCamera::GetViewMatrixInv()
{
	//更新
	Update();
	return viewMatrixInv;
}

const AliceMathF::Matrix4& GameCamera::GetProjectionMatrix()
{
	//更新
	Update();
	return projectionMatrix;
}

const AliceMathF::Matrix4& GameCamera::GetViewProjectionMatrix()
{
	//更新
	Update();
	return viewProjectionMatrix;
}

const AliceMathF::Matrix4& GameCamera::GetCameraRotation()
{
	//更新
	Update();
	return cameraRotation;
}

float GameCamera::GetFar() const
{
	return farClip;
}

float GameCamera::GetNear() const
{
	return nearClip;
}

float GameCamera::GetFovAngleY() const
{
	return fovAngleY;
}

float GameCamera::GetTargetToPositionLength() const
{
	return tgtToPosLen;
}

const AliceMathF::Vector3& GameCamera::GetForward() const
{
	return forward;
}

float GameCamera::GetAspect() const
{
	return aspect;
}

float GameCamera::GetLeft()const
{
	return left;
}

const AliceMathF::Vector3& GameCamera::GetEye()const
{
	return eye;
}

const AliceMathF::Vector3& GameCamera::GetUp()const
{
	return up;
}

const AliceMathF::Vector3& GameCamera::GetTarget()const
{
	return target;
}

float GameCamera::GetRight()const
{
	return right;
}

float GameCamera::GetTop()const
{
	return top;
}

float GameCamera::GetBottom()const
{
	return bottom;
}

#pragma endregion

void Camera::SetWindowsApp(IWindowsApp* windowsApp_)
{
	sWindowsApp = windowsApp_;
}