#include<CreateBlend.h>
#include "Model.h"


void CreateAnimationModelDrawMaterial(MaterialManager* manager_,IAdapter* adapter_)
{
	std::unique_ptr<Material>lMaterial = std::make_unique<Material>();
	ID3D12Device* lDevice = adapter_->GetDevice()->Get();

	//頂点シェーダの読み込み
	lMaterial->vertexShader = CreateUniqueShader("Resources/Shaders/AliceParticle/AnimationModel/VS.hlsl");

	//ジオメトリシェーダの読み込み
	lMaterial->geometryShader = CreateUniqueShader("Resources/Shaders/AliceParticle/AnimationModel/GS.hlsl","main","gs_5_0",IShader::ShaderType::GS);

	//ピクセルシェーダの読み込み
	lMaterial->pixelShader = CreateUniqueShader("Resources/Shaders/AliceParticle/AnimationModel/PS.hlsl","main","ps_5_0",IShader::ShaderType::PS);

	//ルートシグネチャ設定
	lMaterial->rootSignature = CreateUniqueRootSignature();
	lMaterial->rootSignature->Add(IRootSignature::RootType::CBV,0);//b0

	lMaterial->rootSignature->Add(IRootSignature::RangeType::UAV,0);//u0

	lMaterial->rootSignature->Add(IRootSignature::RangeType::SRV,0);//t0
	lMaterial->rootSignature->AddStaticSampler(0);//s0

	lMaterial->rootSignature->Create(lDevice);

	lMaterial->primitiveType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT;

	lMaterial->blenddesc = CreateBlend(BlendMode::AX_BLENDMODE_ADD);

	lMaterial->cullMode = D3D12_CULL_MODE_NONE;

	lMaterial->depthFlag = true;

	lMaterial->depthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;

	//生成
	lMaterial->Initialize();

	manager_->AddMaterial(lMaterial,"AnimationModelGPUParticleDraw");
}

void CreateAnimationModelEmitComputeMaterial(MaterialManager* manager_,IAdapter* adapter_)
{
	std::unique_ptr<ComputeMaterial> lMaterial = std::make_unique<ComputeMaterial>();
	ID3D12Device* lDevice = adapter_->GetDevice()->Get();

	//頂点シェーダの読み込み
	lMaterial->computeShader = CreateUniqueShader("Resources/Shaders/AliceParticle/AnimationModel/EmitCS.hlsl","main","cs_5_0",IShader::ShaderType::CS);

	//ルートシグネチャ設定
	lMaterial->rootSignature = CreateUniqueRootSignature();

	lMaterial->rootSignature->Add(IRootSignature::RootType::CBV,0);//b0
	lMaterial->rootSignature->Add(IRootSignature::RootType::CBV,1);//b1

	lMaterial->rootSignature->Add(IRootSignature::RangeType::SRV,0);//t0

	lMaterial->rootSignature->Add(IRootSignature::RangeType::UAV,0);//u0

	lMaterial->rootSignature->Create(lDevice);

	//生成
	lMaterial->Initialize(adapter_->GetDevice());

	manager_->AddMaterial(lMaterial,"ComputeAnimationModelGPUParticleEmit");
}

void CreateAnimationModelFreeListInitComputeMaterial(MaterialManager* manager_,IAdapter* adapter_)
{
	std::unique_ptr<ComputeMaterial> lMaterial = std::make_unique<ComputeMaterial>();
	ID3D12Device* lDevice = adapter_->GetDevice()->Get();

	//シェーダの読み込み
	lMaterial->computeShader = CreateUniqueShader("Resources/Shaders/AliceParticle/AnimationModel/FreeListInitializeCS.hlsl","main","cs_5_0",IShader::ShaderType::CS);

	//ルートシグネチャ設定
	lMaterial->rootSignature = CreateUniqueRootSignature();

	lMaterial->rootSignature->Add(IRootSignature::RangeType::UAV,0);//u0

	lMaterial->rootSignature->Add(IRootSignature::RootType::CBV,0);//b0

	lMaterial->rootSignature->Create(lDevice);

	//生成
	lMaterial->Initialize(adapter_->GetDevice());

	manager_->AddMaterial(lMaterial,"ComputeAnimationModelGPUParticleFreeListInit");
}

void CreateAnimationModelDrawListReleaseComputeMaterial(MaterialManager* manager_,IAdapter* adapter_)
{
	std::unique_ptr<ComputeMaterial> lMaterial = std::make_unique<ComputeMaterial>();
	ID3D12Device* lDevice = adapter_->GetDevice()->Get();

	//シェーダの読み込み
	lMaterial->computeShader = CreateUniqueShader("Resources/Shaders/AliceParticle/AnimationModel/DrawListRelease.hlsl","main","cs_5_0",IShader::ShaderType::CS);

	//ルートシグネチャ設定
	lMaterial->rootSignature = CreateUniqueRootSignature();

	lMaterial->rootSignature->Add(IRootSignature::RangeType::UAV,0);//u0
	lMaterial->rootSignature->Add(IRootSignature::RangeType::UAV,1);//u1

	lMaterial->rootSignature->Add(IRootSignature::RootType::CBV,0);//b0

	lMaterial->rootSignature->Create(lDevice);

	//生成
	lMaterial->Initialize(adapter_->GetDevice());

	manager_->AddMaterial(lMaterial,"ComputeAnimationModelGPUParticleDrawListRelease");
}

void CreateAnimationModelUpdateComputeMaterial(MaterialManager* manager_,IAdapter* adapter_)
{
	std::unique_ptr<ComputeMaterial> lMaterial = std::make_unique<ComputeMaterial>();
	ID3D12Device* lDevice = adapter_->GetDevice()->Get();

	//頂点シェーダの読み込み
	lMaterial->computeShader = CreateUniqueShader("Resources/Shaders/AliceParticle/AnimationModel/UpdateCS.hlsl","main","cs_5_0",IShader::ShaderType::CS);

	//ルートシグネチャ設定
	lMaterial->rootSignature = CreateUniqueRootSignature();

	lMaterial->rootSignature->Add(IRootSignature::RootType::CBV,0);//b0
	lMaterial->rootSignature->Add(IRootSignature::RootType::CBV,1);//b1

	lMaterial->rootSignature->Add(IRootSignature::RangeType::UAV,0);//t0

	lMaterial->rootSignature->Create(lDevice);

	//生成
	lMaterial->Initialize(adapter_->GetDevice());

	manager_->AddMaterial(lMaterial,"ComputeAnimationModelGPUParticleUpdate");
}

void CreateAnimationModelDrawArgumentUpdateComputeMaterial(MaterialManager* manager_,IAdapter* adapter_)
{
	std::unique_ptr<ComputeMaterial> lMaterial = std::make_unique<ComputeMaterial>();
	ID3D12Device* lDevice = adapter_->GetDevice()->Get();

	//シェーダの読み込み
	lMaterial->computeShader = CreateUniqueShader("Resources/Shaders/AliceParticle/AnimationModel/DrawArgumentUpdateCS.hlsl","main","cs_5_0",IShader::ShaderType::CS);

	//ルートシグネチャ設定
	lMaterial->rootSignature = CreateUniqueRootSignature();

	lMaterial->rootSignature->Add(IRootSignature::RootType::CBV,0);//b0

	lMaterial->rootSignature->Add(IRootSignature::RangeType::UAV,0);//u0

	lMaterial->rootSignature->Create(lDevice);

	//生成
	lMaterial->Initialize(adapter_->GetDevice());

	manager_->AddMaterial(lMaterial,"ComputeAnimationModelGPUParticleDrawArgumentUpdate");
}


void CreateModelDrawMaterial(MaterialManager* manager_,IAdapter* adapter_)
{
	std::unique_ptr<Material>lMaterial = std::make_unique<Material>();
	ID3D12Device* lDevice = adapter_->GetDevice()->Get();

	//頂点シェーダの読み込み
	lMaterial->vertexShader = CreateUniqueShader("Resources/Shaders/AliceParticle/Model/VS.hlsl");

	//ジオメトリシェーダの読み込み
	lMaterial->geometryShader = CreateUniqueShader("Resources/Shaders/AliceParticle/Model/GS.hlsl","main","gs_5_0",IShader::ShaderType::GS);

	//ピクセルシェーダの読み込み
	lMaterial->pixelShader = CreateUniqueShader("Resources/Shaders/AliceParticle/Model/PS.hlsl","main","ps_5_0",IShader::ShaderType::PS);

	//ルートシグネチャ設定
	lMaterial->rootSignature = CreateUniqueRootSignature();
	lMaterial->rootSignature->Add(IRootSignature::RootType::CBV,0);//b0

	lMaterial->rootSignature->Add(IRootSignature::RangeType::UAV,0);//u0

	lMaterial->rootSignature->Add(IRootSignature::RangeType::SRV,0);//t0
	lMaterial->rootSignature->AddStaticSampler(0);//s0

	lMaterial->rootSignature->Create(lDevice);

	lMaterial->primitiveType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT;

	lMaterial->blenddesc = CreateBlend(BlendMode::AX_BLENDMODE_ADD);

	lMaterial->cullMode = D3D12_CULL_MODE_NONE;

	lMaterial->depthFlag = true;

	lMaterial->depthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;

	//生成
	lMaterial->Initialize();

	manager_->AddMaterial(lMaterial,"ModelGPUParticleDraw");
}

void CreateModelEmitComputeMaterial(MaterialManager* manager_,IAdapter* adapter_)
{
	std::unique_ptr<ComputeMaterial> lMaterial = std::make_unique<ComputeMaterial>();
	ID3D12Device* lDevice = adapter_->GetDevice()->Get();

	//頂点シェーダの読み込み
	lMaterial->computeShader = CreateUniqueShader("Resources/Shaders/AliceParticle/Model/EmitCS.hlsl","main","cs_5_0",IShader::ShaderType::CS);

	//ルートシグネチャ設定
	lMaterial->rootSignature = CreateUniqueRootSignature();

	lMaterial->rootSignature->Add(IRootSignature::RootType::CBV,0);//b0
	lMaterial->rootSignature->Add(IRootSignature::RootType::CBV,1);//b1
	lMaterial->rootSignature->Add(IRootSignature::RootType::CBV,2);//b2
	lMaterial->rootSignature->Add(IRootSignature::RootType::CBV,3);//b3

	lMaterial->rootSignature->Add(IRootSignature::RangeType::SRV,0);//t0

	lMaterial->rootSignature->Add(IRootSignature::RangeType::UAV,0);//u0

	lMaterial->rootSignature->Create(lDevice);

	//生成
	lMaterial->Initialize(adapter_->GetDevice());

	manager_->AddMaterial(lMaterial,"ComputeModelGPUParticleEmit");
}

void CreateModelFreeListInitComputeMaterial(MaterialManager* manager_,IAdapter* adapter_)
{
	std::unique_ptr<ComputeMaterial> lMaterial = std::make_unique<ComputeMaterial>();
	ID3D12Device* lDevice = adapter_->GetDevice()->Get();

	//シェーダの読み込み
	lMaterial->computeShader = CreateUniqueShader("Resources/Shaders/AliceParticle/Model/FreeListInitializeCS.hlsl","main","cs_5_0",IShader::ShaderType::CS);

	//ルートシグネチャ設定
	lMaterial->rootSignature = CreateUniqueRootSignature();

	lMaterial->rootSignature->Add(IRootSignature::RangeType::UAV,0);//u0

	lMaterial->rootSignature->Add(IRootSignature::RootType::CBV,0);//b0

	lMaterial->rootSignature->Create(lDevice);

	//生成
	lMaterial->Initialize(adapter_->GetDevice());

	manager_->AddMaterial(lMaterial,"ComputeModelGPUParticleFreeListInit");
}

void CreateModelDrawListReleaseComputeMaterial(MaterialManager* manager_,IAdapter* adapter_)
{
	std::unique_ptr<ComputeMaterial> lMaterial = std::make_unique<ComputeMaterial>();
	ID3D12Device* lDevice = adapter_->GetDevice()->Get();

	//シェーダの読み込み
	lMaterial->computeShader = CreateUniqueShader("Resources/Shaders/AliceParticle/Model/DrawListRelease.hlsl","main","cs_5_0",IShader::ShaderType::CS);

	//ルートシグネチャ設定
	lMaterial->rootSignature = CreateUniqueRootSignature();

	lMaterial->rootSignature->Add(IRootSignature::RangeType::UAV,0);//u0

	lMaterial->rootSignature->Add(IRootSignature::RootType::CBV,0);//b0

	lMaterial->rootSignature->Create(lDevice);

	//生成
	lMaterial->Initialize(adapter_->GetDevice());

	manager_->AddMaterial(lMaterial,"ComputeModelGPUParticleDrawListRelease");
}

void CreateModelUpdateComputeMaterial(MaterialManager* manager_,IAdapter* adapter_)
{
	std::unique_ptr<ComputeMaterial> lMaterial = std::make_unique<ComputeMaterial>();
	ID3D12Device* lDevice = adapter_->GetDevice()->Get();

	//頂点シェーダの読み込み
	lMaterial->computeShader = CreateUniqueShader("Resources/Shaders/AliceParticle/Model/UpdateCS.hlsl","main","cs_5_0",IShader::ShaderType::CS);

	//ルートシグネチャ設定
	lMaterial->rootSignature = CreateUniqueRootSignature();

	lMaterial->rootSignature->Add(IRootSignature::RootType::CBV,0);//b0
	lMaterial->rootSignature->Add(IRootSignature::RootType::CBV,1);//b1
	lMaterial->rootSignature->Add(IRootSignature::RootType::CBV,2);//u2
	lMaterial->rootSignature->Add(IRootSignature::RootType::CBV,3);//u3

	lMaterial->rootSignature->Add(IRootSignature::RangeType::UAV,0);//u0

	lMaterial->rootSignature->Add(IRootSignature::RangeType::SRV,0);//t0

	lMaterial->rootSignature->Create(lDevice);

	//生成
	lMaterial->Initialize(adapter_->GetDevice());

	manager_->AddMaterial(lMaterial,"ComputeModelGPUParticleUpdate");
}

void CreateModelDrawArgumentUpdateComputeMaterial(MaterialManager* manager_,IAdapter* adapter_)
{
	std::unique_ptr<ComputeMaterial> lMaterial = std::make_unique<ComputeMaterial>();
	ID3D12Device* lDevice = adapter_->GetDevice()->Get();

	//シェーダの読み込み
	lMaterial->computeShader = CreateUniqueShader("Resources/Shaders/AliceParticle/Model/DrawArgumentUpdateCS.hlsl","main","cs_5_0",IShader::ShaderType::CS);

	//ルートシグネチャ設定
	lMaterial->rootSignature = CreateUniqueRootSignature();

	lMaterial->rootSignature->Add(IRootSignature::RootType::CBV,0);//b0

	lMaterial->rootSignature->Add(IRootSignature::RangeType::UAV,0);//u0

	lMaterial->rootSignature->Create(lDevice);

	//生成
	lMaterial->Initialize(adapter_->GetDevice());

	manager_->AddMaterial(lMaterial,"ComputeModelGPUParticleDrawArgumentUpdate");
}