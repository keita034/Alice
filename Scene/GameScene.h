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

class GameScene : public BaseScene
{
private:

	enum class DemoIndex
	{
		SKINNING_ANIMATION,
		PHONG_SHADER,
		TOON_SHADER,
		POSTEFFECT,
	};

	std::vector<const char*> demoList{{"SKINNING_ANIMATION"}, { "PHONG_SHADER" },  {"TOON_SHADER"}, {"POSTEFFECT"} };
	int32_t demoIndex;

	std::unique_ptr<Light> light;
	AliceMathF::Vector4 lightColor = { 1, 1, 1, 1 };

	std::unique_ptr<GameCamera> camera;

	std::unique_ptr<AliceModel>skinningModel;
	uint32_t skinningModelHandle;
	Transform skinningModelTransform;
	float modelAnimationFrame = 0.0f;

	std::unique_ptr<AliceMotionData> modelAnimation;
	uint32_t modelAnimationHandle;

	std::unique_ptr<AliceModel>sphereModel;
	std::unique_ptr<AliceToonModel>sphereToonModel;
	uint32_t sphereModelHandle;
	uint32_t sphereToonModelHandle;
	Transform sphereModelTransform;


	std::unique_ptr<AliceModel>posteffectModel;
	uint32_t posteffectModelHandle;
	Transform posteffectModelTransform;

	std::array<bool, 2>stripedPosteffect;
	std::array<bool, 2>gaussianBlurPosteffect;
	std::array<bool, 2>oilPaintingPostEffect;
	std::array<bool, 2>chromaticAberrationPostEffect;
	std::array<bool, 2>vignettePostEffect;

	std::array<bool, 2>bloomPostEffect;
	std::array<bool, 2>sandStormPostEffect;
	std::array<bool, 2>crtPostEffect;
	std::array<bool, 2>monochromePostEffect;

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

	void PostEffectGui();

private:
	//コピーコンストラクタ・代入演算子削除
	GameScene& operator=(const GameScene&) = delete;
	GameScene(const GameScene&) = delete;
};