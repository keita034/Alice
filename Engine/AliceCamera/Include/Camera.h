#pragma once
#include"AliceMathUtility.h"
#include"WindowsApp.h"

///<summary>
///射影行列の更新方法。
///</summary>
enum UpdateProjMatrixFunc
{
	UPDATE_PROJMATRIXFUNC_PERSPECTIVE,		//透視射影行列。遠近法が効いた絵を作りたいならこっち。
	UPDATE_PROJMATRIXFUNC_ORTHO,			//平行投影。２Ｄ的な表現がしたいならこっち。
};

class Camera
{
protected:
	static IWindowsApp* sWindowsApp;
	static AliceMathF::Matrix4* viewMatrixPtr;
	static AliceMathF::Matrix4* projectionMatrixPtr;
	static Camera* cameraPtr;

public:

	Camera() = default;
	virtual ~Camera() = default;

	/// <summary>
	/// ビュー行列を取得
	/// </summary>
	virtual const AliceMathF::Matrix4& GetViewMatrix() = 0;

	/// <summary>
	/// ビュー行列の逆行列を取得
	/// </summary>
	virtual const AliceMathF::Matrix4& GetViewMatrixInv() = 0;

	/// <summary>
	/// プロジェクション行列を取得
	/// </summary>
	virtual const AliceMathF::Matrix4& GetProjectionMatrix() = 0;

	/// <summary>
	/// ビュー×プロジェクション行列を取得
	/// </summary>
	virtual const AliceMathF::Matrix4& GetViewProjectionMatrix() = 0;

	/// <summary>
	/// カメラの回転行列を取得
	/// </summary>
	virtual const AliceMathF::Matrix4& GetCameraRotation() = 0;

	virtual const AliceMathF::Matrix4& GetBillboardMatrix() = 0;


	/// <summary>
	/// 遠平面までの距離を取得
	/// </summary>
	virtual float GetFar() const = 0;

	/// <summary>
	/// 近平面までの距離を取得
	/// </summary>
	virtual float GetNear() const = 0;

	/// <summary>
	/// アスペクト比を取得
	/// </summary>
	virtual float GetAspect() const = 0;

	/// <summary>
	/// 画角を取得
	/// </summary>
	/// <returns>画角。単位ラジアン</returns>
	virtual float GetFovAngleY() const = 0;

	/// <summary>
	/// 注視点と視点の距離を取得
	/// </summary>
	/// <returns></returns>
	virtual float GetTargetToPositionLength() const = 0;

	/// <summary>
	/// カメラの前方向を取得
	/// </summary>
	virtual const AliceMathF::Vector3& GetForward() const = 0;

	/// <summary>
	/// 平行投影の下端を取得
	/// </summary>
	virtual float GetBottom() const = 0;

	/// <summary>
	/// 平行投影の上端を取得
	/// </summary>
	virtual float GetTop() const = 0;

	/// <summary>
	/// 平行投影の右端を取得
	/// </summary>
	virtual float GetRight() const = 0;

	/// <summary>
	/// 平行投影の左端を取得
	/// </summary>
	virtual float GetLeft() const = 0;

	/// <summary>
	/// 視点座標を取得
	/// </summary>
	virtual const AliceMathF::Vector3& GetEye() const = 0;

	/// <summary>
	/// 注視点座標を取得
	/// </summary>
	virtual const AliceMathF::Vector3& GetUp() const = 0;

	/// <summary>
	/// 上方向ベクトルを取得
	/// </summary>
	virtual const AliceMathF::Vector3& GetTarget() const = 0;

	static void SSetWindowsApp(IWindowsApp* windowsApp_);

	static AliceMathF::Matrix4* GetViewMatrixPtr();
	static AliceMathF::Matrix4* GetProjectionMatrixPtr();
	static Camera* GetCameraPtr();
};

