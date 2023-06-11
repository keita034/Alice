#include "Camera.h"

IWindowsApp* Camera::windowsApp = nullptr;

void GameCamera::Initialize(UpdateProjMatrixFunc matFunc)
{
	//アスペクト比を計算する
	aspect =
		static_cast<float>(windowsApp->GetWindowSize().width) /
		static_cast<float>(windowsApp->GetWindowSize().height);

	matrixFunc = matFunc;

	if (matFunc == UpdateProjMatrixFunc_Ortho)//平行投影
	{
		if (near_ == 0.0f)
		{
			near_ = 0.1f;
		}
		if (far_ == 0.0f)
		{
			far_ = 1000.0f;
		}
		right = static_cast<float>(windowsApp->GetWindowSize().width);
		bottom = static_cast<float>(windowsApp->GetWindowSize().height);

		//平行投影の計算
		AliceMathF::MakeOrthogonalL(right, bottom, near_, far_, projectionMatrix);

	}
	else//透視投影
	{
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
			fovAngleY = AliceMathF::AX_PI / 4;
		}

		//透視投影行列の計算
		AliceMathF::MakePerspectiveL(fovAngleY, aspect, near_, far_, projectionMatrix);

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
	AliceMathF::Vector3 toPos;
	toPos = eye - target;
	tgtToPosLen = toPos.length_();

	updateViewMatrix = false;
}

void GameCamera::Update()
{
	if (updateProjMatrix)
	{
		if (matrixFunc == UpdateProjMatrixFunc_Ortho)
		{

			right = static_cast<float>(windowsApp->GetWindowSize().width);
			bottom = static_cast<float>(windowsApp->GetWindowSize().height);
			AliceMathF::MakeOrthogonalL(right, bottom, near_, far_, projectionMatrix);

		}
		else
		{
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
				fovAngleY = AliceMathF::AX_PI / 4;
			}

			AliceMathF::MakePerspectiveL(fovAngleY, aspect, near_, far_, projectionMatrix);
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

		AliceMathF::Vector3 toPos;
		toPos = eye - target;
		tgtToPosLen = toPos.length_();

		updateViewMatrix = false;
	}
}

void GameCamera::Move(const AliceMathF::Vector3& move)
{
	eye += move;
	target += move;
	updateViewMatrix = true;
}

void GameCamera::MoveTarget(const AliceMathF::Vector3& move)
{
	target += move;
	updateViewMatrix = true;
}

void GameCamera::MovePosition(const AliceMathF::Vector3& move)
{
	eye += move;
	updateViewMatrix = true;
}

#pragma region セッター

void GameCamera::SetAspect(float aspect_)
{
	aspect = aspect_;
	updateViewMatrix = true;
}

void GameCamera::SetFar(float fFar)
{
	far_ = fFar;
	updateProjMatrix = true;
}

void GameCamera::SetNear(float fNear)
{
	near_ = fNear;
	updateProjMatrix = true;
}

void GameCamera::SetFovAngleY(float fovAngle)
{
	this->fovAngleY = fovAngle;
	updateProjMatrix = true;
}

void GameCamera::SetUpdateProjMatrixFunc(UpdateProjMatrixFunc func)
{
	matrixFunc = func;
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

void GameCamera::SetEye(const AliceMathF::Vector3& pos)
{
	updateViewMatrix = true;
	eye = pos;
}

void GameCamera::SetTarget(const AliceMathF::Vector3& pos)
{
	updateViewMatrix = true;
	target = pos;
}

void GameCamera::SetUp(const AliceMathF::Vector3& vec)
{
	updateViewMatrix = true;
	up = vec;
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
	return far_;
}

float GameCamera::GetNear() const
{
	return near_;
}

float GameCamera::GetFovAngleY() const
{
	return fovAngleY;
}

float GameCamera::GetTargetToPositionlength_() const
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

float GameCamera::GetLeft()
{
	return left;
}

const AliceMathF::Vector3& GameCamera::GetEye()
{
	return eye;
}

const AliceMathF::Vector3& GameCamera::GetUp()
{
	return up;
}

const AliceMathF::Vector3& GameCamera::GetTarget()
{
	return target;
}

float GameCamera::GetRight()
{
	return right;
}

float GameCamera::GetTop()
{
	return top;
}

float GameCamera::GetBottom()
{
	return bottom;
}

#pragma endregion