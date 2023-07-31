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

#include<FadeOutTransition.h>

class TitleScene : public BaseScene
{
private:

	std::unique_ptr<GameCamera> camera_;

	std::unique_ptr<Sprite2D>title;
	uint32_t titleHanlde = 0;
	Transform transform;

	std::unique_ptr<Sprite2D>startA;
	uint32_t startAHanlde = 0;
	Transform startATransform;

	bool sceneChange = false;

	std::unique_ptr<ITransition>transition;
public:

	TitleScene() = default;
	~TitleScene() = default;

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
	TitleScene& operator=(const TitleScene&) = delete;
	TitleScene(const TitleScene&) = delete;
};

