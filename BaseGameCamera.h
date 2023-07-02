#pragma once
#include<Camera.h>
#include<Transform.h>

class BaseGameCamera
{
protected:

	Transform transform;

	std::unique_ptr<GameCamera> camera;

	AliceMathF::Quaternion quaternion;

public:

	void CameraUpdate();

	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Initialize()=0;

	/// <summary>
	/// 更新処理
	/// </summary>
	virtual void Update() = 0;

	/// <summary>
	/// 最初の方に持ってきたい更新処理
	/// </summary>
	virtual void BeginUpdate()=0;

	/// <summary>
	/// 描画
	/// </summary>
	virtual void Draw()= 0;

	/// <summary>
	/// 後始末
	/// </summary>
	virtual void Finalize() =0;

	/// <summary>
	/// カメラ取得
	/// </summary>
	BaseGameCamera* GetCamera();

	/// <summary>
	/// カメラ取得
	/// </summary>
	Camera* GetGameCamera();

	BaseGameCamera() = default;
	virtual ~BaseGameCamera();
};

