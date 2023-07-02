#pragma once
#include<vector>

#include<Camera.h>
#include<Input.h>

#include<BaseGameCamera.h>

class Player;

class GameCameraManager
{
public:

	enum CameraIndex
	{
		PLAYER_CAMERA,

		CAMERA_NUM,
	};

private:

	std::vector <std::unique_ptr<BaseGameCamera>>cameras;

	CameraIndex cameraIndex = PLAYER_CAMERA;

	Player* player = nullptr;

public:

	GameCameraManager();
	~GameCameraManager();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(Player* player_, AliceInput::IInput* input_);

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// 最初の方に持ってきたい更新処理
	/// </summary>
	void BeginUpdate();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// 後始末
	/// </summary>
	void Finalize();

	/// <summary>
	/// カメラ取得
	/// </summary>
	Camera* GetCamera();

	/// <summary>
	/// カメラ取得
	/// </summary>
	BaseGameCamera* GetGameCamera();


	CameraIndex GetCameraIndex();

private:

	void PPlayerCameraUpdate();


	//コピーコンストラクタ・代入演算子削除
	GameCameraManager& operator=(const GameCameraManager&) = delete;
	GameCameraManager(const GameCameraManager&) = delete;
};

