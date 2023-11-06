#include "Test.h"

#include<CreateBlend.h>

void CreateTestMaterial(MaterialManager* manager_,IAdapter* adapter_)
{
	std::unique_ptr<Material>lDefaultModelMaterial = std::make_unique<Material>();
	ID3D12Device* lDevice = adapter_->GetDevice()->Get();

	//頂点シェーダの読み込み
	lDefaultModelMaterial->vertexShader = CreateUniqueShader("Resources/Shaders/Test/MultiGPU/TestVS.hlsl");

	//ピクセルシェーダの読み込み
	lDefaultModelMaterial->pixelShader = CreateUniqueShader("Resources/Shaders/Test/MultiGPU/TestPS.hlsl","main","ps_5_0",IShader::ShaderType::PS);

		//頂点レイアウト設定
	lDefaultModelMaterial->inputLayouts =
	{
		// xyz座標
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
	};

	lDefaultModelMaterial->depthFlag = false;

	lDefaultModelMaterial->blenddesc = CreateBlend(BlendMode::AX_BLENDMODE_NOBLEND);

	lDefaultModelMaterial->cullMode = D3D12_CULL_MODE_NONE;

	//ルートシグネチャ設定
	lDefaultModelMaterial->rootSignature = CreateUniqueRootSignature();
	lDefaultModelMaterial->rootSignature->Add(IRootSignature::RangeType::UAV,0);//t0
	lDefaultModelMaterial->rootSignature->Create(lDevice);
	//生成
	lDefaultModelMaterial->Initialize();

	manager_->AddMaterial(lDefaultModelMaterial,"MultiGPUTest");
}

void CreateTestComputeMaterial(MaterialManager* manager_,IAdapter* adapter_)
{
	std::unique_ptr<ComputeMaterial> lBasicGPUParticleEmitComputeMaterial = std::make_unique<ComputeMaterial>();
	ID3D12Device* lDevice = adapter_->GetDevice()->Get();

	//シェーダの読み込み
	lBasicGPUParticleEmitComputeMaterial->computeShader = CreateUniqueShader("Resources/Shaders/Test/MultiGPU/TestCS.hlsl","main","cs_5_0",IShader::ShaderType::CS);

	//ルートシグネチャ設定
	lBasicGPUParticleEmitComputeMaterial->rootSignature = CreateUniqueRootSignature();

	lBasicGPUParticleEmitComputeMaterial->rootSignature->Add(IRootSignature::RangeType::UAV,0);//u0

	lBasicGPUParticleEmitComputeMaterial->rootSignature->Create(lDevice);

	//生成
	lBasicGPUParticleEmitComputeMaterial->Initialize(adapter_->GetDevice());

	manager_->AddMaterial(lBasicGPUParticleEmitComputeMaterial,"ComputeMultiGPUTest");
}
