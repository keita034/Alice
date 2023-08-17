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
#include<FadeInTransition.h>

class FailureScene : public BaseScene
{
private:

	std::unique_ptr<Sprite2D>failure;
	uint32_t failureHanlde = 0;
	Transform transform;


	std::unique_ptr<ITransition>inTransition;
	std::unique_ptr<ITransition>outTransition;

	bool sceneChange = false;

public:
	FailureScene() = default;
	~FailureScene() = default;

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
	FailureScene& operator=(const FailureScene&) = delete;
	FailureScene(const FailureScene&) = delete;
};

