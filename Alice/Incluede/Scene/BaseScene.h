#pragma once
#include"Input.h"
/// <summary>
/// シーン基底クラス
/// </summary>

class SceneManager;

class BaseScene
{
private:

	static AliceInput::IInput* input;

	//シーンマネージャー
	SceneManager* sceneManager = nullptr;

public:

	virtual ~BaseScene() = default;

	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Initialize() = 0;

	/// <summary>
	/// 更新処理
	/// </summary>
	virtual void Update() = 0;

	/// <summary>
	/// 描画
	/// </summary>
	virtual void Draw() = 0;

	/// <summary>
	/// 後始末
	/// </summary>
	virtual void Finalize() = 0;

	virtual void SetSceneManager(SceneManager* manager);

	static void SetInput(AliceInput::IInput* iInput);
};

