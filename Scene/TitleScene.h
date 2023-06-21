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


class TitleScene : public BaseScene
{
private:

	std::unique_ptr<CinemaCamera> camera_;

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

