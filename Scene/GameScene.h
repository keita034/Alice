#pragma once

#include"Camera.h"
#include"Sprite2D.h"
#include"Sprite3D.h"
#include"BasicParticle.h"
#include"RainParticle.h"
#include"ParticleEmitter.h"
#include"Light.h"
#include"Mesh.h"
#include"3DMesh.h"
#include"Transform.h"
#include"DefaultMaterial.h"
#include"AudioManager.h"
#include"AliceModel.h"
#include "CinemaCamera.h"
#include "BaseScene.h"
#include "Light.h"
#include<PostEffectManager.h>

class GameScene : public BaseScene
{
private:

	Mesh* mesh2D = nullptr;
	Mesh3D* mesh3D = nullptr;
	AudioManager* audioManager = nullptr;

	std::unique_ptr<Light> light;
	AliceMathF::Vector4 lightColor = { 1, 1, 1, 1 };

	std::unique_ptr<GameCamera> camera;

	std::unique_ptr<AliceModel> playerModel;
	uint32_t playerModelHandle;
	Transform playerTransform;

public:

	GameScene();
	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize() override;

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update() override;

	/// <summary>
	/// 描画
	/// </summary>
	void Draw() override;

	/// <summary>
	/// 後始末
	/// </summary>
	void Finalize() override;

private:

	//コピーコンストラクタ・代入演算子削除
	GameScene& operator=(const GameScene&) = delete;
	GameScene(const GameScene&) = delete;
};