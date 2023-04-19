#pragma once
#include<BasePostEffect.h>
#include<ConstantBuffer.h>
#include<AliceMathUtility.h>
#include<GaussianYBlurPostEffect.h>
#include<GaussianXBlurPostEffect.h>
#include<DirectX12Core.h>
#include<WindowsApp.h>

class StripedPostEffect : public BasePostEffect
{
private:

	std::unique_ptr<RenderTarget>renderTarget;

public:

	/// <summary>
	/// ������
	/// </summary>
	void Initialize() override;
	
	/// <summary>
	/// �X�V
	/// </summary>
	void PostUpdate(RenderTarget* mainRenderTarget) override;

	/// <summary>
	/// �^�C�v���擾
	/// </summary>
	const std::string& GetType()override;

private:

	//�`��
	void Draw(RenderTarget* mainRenderTarget)override;

	void MainRenderTargetDraw(RenderTarget* mainRenderTarget)override;

	StripedPostEffect() = default;
	~StripedPostEffect() = default;

	//�R�s�[�R���X�g���N�^�E������Z�q�폜
	StripedPostEffect& operator=(const StripedPostEffect&) = delete;
	StripedPostEffect(const StripedPostEffect&) = delete;
};

