#pragma once
#include"Camera.h"
#include"Sprite2D.h"
#include"Sprite3D.h"
#include"Input.h"
#include"BasicParticle.h"
#include"RainParticle.h"
#include"ParticleEmitter.h"
#include"Light.h"
#include"Mesh.h"
#include"3DMesh.h"
#include"Transform.h"
#include"DefaultMaterial.h"
#include"AudioManager.h"
#include"Button.h"
#include"Cursor.h"
#include"AliceModel.h"
#include "CinemaCamera.h"
#include "BaseScene.h"
#include "Light.h"


class ResultScene : public BaseScene
{
private:
	Input* input = nullptr;
	
public:
	ResultScene() = default;
	~ResultScene() = default;

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
	ResultScene& operator=(const ResultScene&) = delete;
	ResultScene(const ResultScene&) = delete;
};

