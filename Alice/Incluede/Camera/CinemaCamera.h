#pragma once
#include"ErrorException.h"
#include"AliceMathUtility.h"
#include"Camera.h"

enum class SensorSize
{
	MEDIUM_FORMAT,//中判
	FF,//フルサイズ
	APS_C,//APS-C
	M_FOUR_THIRDS,//マイクロフォーサーズ
	ONE_INCH,//1インチ
	ONE_OVER_ONE_POINT_SEVEN_INCH,//1/1.7インチ
	ONE_THIRD_INCH//1/3インチ
};

struct CinemaCameraSensorSize
{
	float width;
	float height;
};

class CinemaCamera :public Camera
{
private:

	//視点座標
	AliceMathF::Vector3 eye = { 0.0f,0.0f,-50.0f };
	//注視点座標
	AliceMathF::Vector3 target = { 0.0f,0.0f,0.0f };
	//上方向ベクトル
	AliceMathF::Vector3 up = { 0.0f,1.0f,0.0f };

	//ビュー行列
	AliceMathF::Matrix4 viewMatrix;
	//プロジェクション行列
	AliceMathF::Matrix4 projectionMatrix;
	//ビュープロジェクション行列
	AliceMathF::Matrix4 viewProjectionMatrix;
	//ビュー行列の逆行列
	AliceMathF::Matrix4 viewMatrixInv;
	//カメラの回転行列
	AliceMathF::Matrix4 cameraRotation;

	//画角(ラジアン度)
	float fovAngleY = 0.0;
	//アスペクト比(画面横幅/画面縦幅)
	float aspect = 0.0f;
	//ニアクリップ(前端)
	float near_ = 0.0f;
	//ファークリップ(奥端)
	float far_ = 0.0f;

	//プロジェクション行列の更新フラグ
	bool updateProjMatrix = true;
	//ビュー行列の更新フラグ
	bool updateViewMatrix = true;
	char PADDING[2]{};

	//注視点と視点の距離
	float tgtToPosLen = 0.0f;

	//カメラの正面ベクトル
	AliceMathF::Vector3 forward = { 0.0f,0.0f,0.0f };

	bool simpleFollowWithWorldUp = false;
	char PADDING2[3]{};

	//焦点距離
	float focalLength = 70.0f;
	//センサーサイズ
	SensorSize sensorSize = SensorSize::MEDIUM_FORMAT;
	//画角計算フラグ
	bool updatefovAngleY;
	//センサーサイズ
	CinemaCameraSensorSize cameraSensorSize = { 43.8f,32.8f };
	//F値
	float aperture;
	//撮影距離
	float shootingDistance;
public:

	CinemaCamera() = default;
	~CinemaCamera() = default;

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// ビュー行列・プロジェクション行列を更新する
	/// </summary>
	void Update();

	/// <summary>
	/// カメラを動かす
	/// </summary>
	/// <param name="move">動かす量</param>
	void Move(const AliceMathF::Vector3& move);

	/// <summary>
	/// 注視点を動かす
	/// </summary>
	/// <param name="move">移動量</param>
	void MoveTarget(const AliceMathF::Vector3& move);

	/// <summary>
	/// 視点を動かす
	/// </summary>
	/// <param name="move">移動量</param>
	void MovePosition(const AliceMathF::Vector3& move);

#pragma region セッター

	/// <summary>
	/// アスペクト比を設定する
	/// </summary>
	void SetAspect(float aspect_);

	/// <summary>
	/// 遠平面までの距離を設定
	/// </summary>
	void SetFar(float fFar);

	/// <summary>
	/// 近平面までの距離を設定
	/// </summary>
	void SetNear(float fNear);

	/// <summary>
	/// 焦点距離を設定
	/// </summary>
	/// <param name="length">焦点距離</param>
	void SetFocalLength(float length);

	/// <summary>
	/// F値を設定
	/// </summary>
	void SetAperture(float value);

	/// <summary>
	/// センサーの種類を設定
	/// </summary>
	/// <param name="size">種類</param>
	void SetSensorSize(SensorSize size);

	/// <summary>
	/// 撮影距離を設定
	/// </summary>
	void SetShootingDistance(float distance);

	/// <summary>
	/// カメラの座標を設定
	/// </summary>
	/// <param name="pos">座標</param>
	void SetEye(const AliceMathF::Vector3& pos);

	/// <summary>
	/// カメラのターゲット座標を設定
	/// </summary>
	/// <param name="pos">座標</param>
	void SetTarget(const AliceMathF::Vector3& pos);

	/// <summary>
	/// カメラの上ベクトルを設定
	/// </summary>
	/// <param name="vec">ベクトル</param>
	void SetUp(const AliceMathF::Vector3& vec);

#pragma endregion

#pragma region ゲッター

	/// <summary>
	/// ビュー行列を取得
	/// </summary>
	const AliceMathF::Matrix4& GetViewMatrix();

	/// <summary>
	/// ビュー行列の逆行列を取得
	/// </summary>
	const AliceMathF::Matrix4& GetViewMatrixInv();

	/// <summary>
	/// プロジェクション行列を取得
	/// </summary>
	const AliceMathF::Matrix4& GetProjectionMatrix();

	/// <summary>
	/// ビュー×プロジェクション行列を取得
	/// </summary>
	const AliceMathF::Matrix4& GetViewProjectionMatrix();

	/// <summary>
	/// カメラの回転行列を取得
	/// </summary>
	const AliceMathF::Matrix4& GetCameraRotation();

	/// <summary>
	/// 遠平面までの距離を取得
	/// </summary>
	float GetFar() const;

	/// <summary>
	/// 近平面までの距離を取得
	/// </summary>
	float GetNear() const;

	/// <summary>
	/// アスペクト比を取得
	/// </summary>
	float GetAspect() const;

	/// <summary>
	/// 画角を取得
	/// </summary>
	/// <returns>画角。単位ラジアン</returns>
	float GetFovAngleY() const;

	/// <summary>
	/// 注視点と視点の距離を取得
	/// </summary>
	float GetTargetToPositionLength() const;

	/// <summary>
	/// 焦点距離を取得
	/// </summary>
	float GetFocalLength() const;

	/// <summary>
	/// F値を設定
	/// </summary>
	float GetAperture() const;

	/// <summary>
	/// 撮影距離を取得
	/// </summary>
	float GetDistance() const;

	/// <summary>
	/// カメラの前方向を取得
	/// </summary>
	const AliceMathF::Vector3& GetForward() const;

	/// <summary>
	/// 視点座標を取得
	/// </summary>
	const AliceMathF::Vector3& GetEye();

	/// <summary>
	/// 注視点座標を取得
	/// </summary>
	const AliceMathF::Vector3& GetUp();

	/// <summary>
	/// 上方向ベクトルを取得
	/// </summary>
	const AliceMathF::Vector3& GetTarget();

#pragma endregion

private:
	/// <summary>
	/// 平行投影の下端を取得
	/// </summary>
	float GetBottom()
	{
		return float();
	};

	/// <summary>
	/// 平行投影の上端を取得
	/// </summary>
	float GetTop()
	{
		return float();
	};

	/// <summary>
	/// 平行投影の右端を取得
	/// </summary>
	float GetRight()
	{
		return float();
	};

	/// <summary>
	/// 平行投影の左端を取得
	/// </summary>
	float GetLeft()
	{
		return float();
	};
};