#pragma once

#include<3DMesh.h>
#include<AliceModel.h>
#include<AliceToonModel.h>
#include<BaseScene.h>
#include<BasicParticle.h>
#include<Camera.h>
#include<CinemaCamera.h>
#include<MaterialManager.h>
#include<Light.h>
#include<Mesh.h>
#include<ParticleEmitter.h>
#include<PostEffectManager.h>
#include<RainParticle.h>
#include<SceneLoader.h>
#include<Sprite2D.h>
#include<Sprite3D.h>
#include<Transform.h>

#include<FadeOutTransition.h>

class TitleScene : public BaseScene
{
private:

	std::unique_ptr<Sprite2D>title;
	uint32_t titleHanlde = 0;
	Transform transform;

	std::unique_ptr<Sprite2D>startA;
	uint32_t startAHanlde = 0;
	Transform startATransform;

	bool sceneChange = false;

	uint32_t bgmHandle = 0;
	uint32_t seHandle = 0;

	float bgmVolume = 0.008f;
	float seVolume = 0.02f;

	std::unique_ptr<ITransition>inTransition;
	std::unique_ptr<ITransition>outTransition;

	std::unique_ptr<GameCamera>gameCamera;

	AliceMathF::Matrix4 tmp;
	AliceMathF::Matrix4 lMat;

public:

	TitleScene() = default;
	~TitleScene() = default;

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

	const AliceMathF::Matrix4& GetSceneViewMatrix()override;

	const AliceMathF::Matrix4& GetSceneProjectionMatrix()override;

private:
	//コピーコンストラクタ・代入演算子削除
	TitleScene& operator=(const TitleScene&) = delete;
	TitleScene(const TitleScene&) = delete;
};

