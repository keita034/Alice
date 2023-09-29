#pragma once

#include<3DMesh.h>
#include<AliceModel.h>
#include<AliceToonModel.h>
#include<BaseScene.h>
#include<BasicParticle.h>
#include<Camera.h>
#include<CinemaCamera.h>
#include<DefaultMaterial.h>
#include<Light.h>
#include<Mesh.h>
#include<ParticleEmitter.h>
#include<PostEffectManager.h>
#include<RainParticle.h>
#include<SceneLoader.h>
#include<Sprite2D.h>
#include<Sprite3D.h>
#include<Transform.h>
#include<ImGuiManager.h>
#include<FadeInTransition.h>
#include<IRigidBody.h>
#include<SphereShape.h>
#include<BoxShape.h>
#include"TestRigidBody.h"

class GameScene : public BaseScene
{
private:

	std::unique_ptr<GameCamera> camera;
	std::unique_ptr<Light>light;

	TestRigidBody testRigidBody;
	AlicePhysics::IRigidBody* sphereBody;
	AlicePhysics::IShape* sphere;
	AlicePhysics::IRigidBody* boxBody;
	AlicePhysics::IShape* box;

public:

	GameScene();
	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(const std::string& previewSceneName_) override;

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