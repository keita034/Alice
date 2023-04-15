#pragma once
#include"BaseScene.h"
#include "TextureManager.h"
#include "Transform.h"
#include "Sprite2D.h"
#include "Input.h"
#include "CinemaCamera.h"
#include "AliceModel.h"
#include "AliceMotionData.h"
#include "Camera.h"
#include "AudioManager.h"

class TitleScene : public BaseScene
{
private:
	Input* input = nullptr;

	std::unique_ptr<CinemaCamera> camera_;

public:

	TitleScene() = default;
	~TitleScene() = default;

	/// <summary>
	/// ������
	/// </summary>
	void Initialize() override;

	/// <summary>
	/// �X�V����
	/// </summary>
	void Update() override;

	/// <summary>
	/// �`��
	/// </summary>
	void Draw() override;

	/// <summary>
	/// ��n��
	/// </summary>
	void Finalize() override;

private:
	//�R�s�[�R���X�g���N�^�E������Z�q�폜
	TitleScene& operator=(const TitleScene&) = delete;
	TitleScene(const TitleScene&) = delete;
};

