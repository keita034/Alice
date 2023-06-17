#pragma once
#include<AliceMathUtility.h>
#include<Camera.h>

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

#pragma warning(push)
#pragma warning(disable:4263)
#pragma warning(disable:4264)

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
	float nearClip = 0.0f;
	//ファークリップ(奥端)
	float farClip = 0.0f;

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
	float focallength = 70.0f;
	//センサーサイズ
	SensorSize sensorSize = SensorSize::MEDIUM_FORMAT;
	//画角計算フラグ
	bool updatefovAngleY;
	char PDING[3];

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
	/// <param name="move_">動かす量</param>
	void Move(const AliceMathF::Vector3& move_);

	/// <summary>
	/// 注視点を動かす
	/// </summary>
	/// <param name="move_">移動量</param>
	void MoveTarget(const AliceMathF::Vector3& move_);

	/// <summary>
	/// 視点を動かす
	/// </summary>
	/// <param name="move_">移動量</param>
	void MovePosition(const AliceMathF::Vector3& move_);

#pragma region セッター

	/// <summary>
	/// アスペクト比を設定する
	/// </summary>
	void SetAspect(float aspect_);

	/// <summary>
	/// 遠平面までの距離を設定
	/// </summary>
	void SetFar(float far_);

	/// <summary>
	/// 近平面までの距離を設定
	/// </summary>
	void SetNear(float near_);

	/// <summary>
	/// 焦点距離を設定
	/// </summary>
	/// <param name="length_">焦点距離</param>
	void SetFocallength_(float length_);

	/// <summary>
	/// F値を設定
	/// </summary>
	void SetAperture(float value_);

	/// <summary>
	/// センサーの種類を設定
	/// </summary>
	/// <param name="size">種類</param>
	void SetSensorSize(SensorSize size_);

	/// <summary>
	/// 撮影距離を設定
	/// </summary>
	void SetShootingDistance(float distance_);

	/// <summary>
	/// カメラの座標を設定
	/// </summary>
	/// <param name="pos">座標</param>
	void SetEye(const AliceMathF::Vector3& pos_);

	/// <summary>
	/// カメラのターゲット座標を設定
	/// </summary>
	/// <param name="pos">座標</param>
	void SetTarget(const AliceMathF::Vector3& pos_);

	/// <summary>
	/// カメラの上ベクトルを設定
	/// </summary>
	/// <param name="vec">ベクトル</param>
	void SetUp(const AliceMathF::Vector3& vec_);

#pragma endregion

#pragma region ゲッター

	/// <summary>
	/// ビュー行列を取得
	/// </summary>
	const AliceMathF::Matrix4& GetViewMatrix() override;

	/// <summary>
	/// ビュー行列の逆行列を取得
	/// </summary>
	const AliceMathF::Matrix4& GetViewMatrixInv() override;

	/// <summary>
	/// プロジェクション行列を取得
	/// </summary>
	const AliceMathF::Matrix4& GetProjectionMatrix()override;

	/// <summary>
	/// ビュー×プロジェクション行列を取得
	/// </summary>
	const AliceMathF::Matrix4& GetViewProjectionMatrix()override;

	/// <summary>
	/// カメラの回転行列を取得
	/// </summary>
	const AliceMathF::Matrix4& GetCameraRotation()override;

	/// <summary>
	/// 遠平面までの距離を取得
	/// </summary>
	float GetFar() const override;

	/// <summary>
	/// 近平面までの距離を取得
	/// </summary>
	float GetNear() const override;

	/// <summary>
	/// アスペクト比を取得
	/// </summary>
	float GetAspect() const override;

	/// <summary>
	/// 画角を取得
	/// </summary>
	/// <returns>画角。単位ラジアン</returns>
	float GetFovAngleY() const override;

	/// <summary>
	/// 注視点と視点の距離を取得
	/// </summary>
	float GetTargetToPositionLength() const override;

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
	const AliceMathF::Vector3& GetEye()const override;

	/// <summary>
	/// 注視点座標を取得
	/// </summary>
	const AliceMathF::Vector3& GetUp()const override;

	/// <summary>
	/// 上方向ベクトルを取得
	/// </summary>
	const AliceMathF::Vector3& GetTarget()const override;

#pragma endregion

private:
	/// <summary>
	/// 平行投影の下端を取得
	/// </summary>
	float GetBottom()const override
	{
		return float();
	};

	/// <summary>
	/// 平行投影の上端を取得
	/// </summary>
	float GetTop()const override
	{
		return float();
	};

	/// <summary>
	/// 平行投影の右端を取得
	/// </summary>
	float GetRight()const override
	{
		return float();
	};

	/// <summary>
	/// 平行投影の左端を取得
	/// </summary>
	float GetLeft()const override
	{
		return float();
	};
};

#pragma warning(pop)