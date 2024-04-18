#include "BloodGush.h"

#include<CreateBlend.h>

void CreateBloodGushDrawMaterial(MaterialManager* manager_,IAdapter* adapter_)
{
	std::unique_ptr<Material>lMaterial = std::make_unique<Material>();
	ID3D12Device* lDevice = adapter_->GetDevice()->Get();

	//頂点シェーダの読み込み
	lMaterial->vertexShader = CreateUniqueShader("Resources/Shaders/AliceParticle/BloodGush/VS.hlsl");

	//ジオメトリシェーダの読み込み
	lMaterial->geometryShader = CreateUniqueShader("Resources/Shaders/AliceParticle/BloodGush/GS.hlsl","main","gs_5_0",IShader::ShaderType::GS);

	//ピクセルシェーダの読み込み
	lMaterial->pixelShader = CreateUniqueShader("Resources/Shaders/AliceParticle/BloodGush/PS.hlsl","main","ps_5_0",IShader::ShaderType::PS);

	//ルートシグネチャ設定
	lMaterial->rootSignature = CreateUniqueRootSignature();
	lMaterial->rootSignature->Add(IRootSignature::RootType::CBV,0);//b0

	lMaterial->rootSignature->Add(IRootSignature::RangeType::UAV,0);//u0
	lMaterial->rootSignature->Add(IRootSignature::RangeType::UAV,1);//u1

	lMaterial->rootSignature->Add(IRootSignature::RangeType::SRV,0);//t0
	lMaterial->rootSignature->AddStaticSampler(0);//s0

	lMaterial->rootSignature->Create(lDevice);

	lMaterial->primitiveType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT;

	lMaterial->blenddesc = CreateBlend(BlendMode::AX_BLENDMODE_SUB);

	lMaterial->cullMode = D3D12_CULL_MODE_NONE;

	lMaterial->depthFlag = true;

	lMaterial->depthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;

	//生成
	lMaterial->Initialize();

	manager_->AddMaterial(lMaterial,"BloodGushGPUParticleDraw");
}

void CreateBloodGushEmitComputeMaterial(MaterialManager* manager_,IAdapter* adapter_)
{
	std::unique_ptr<ComputeMaterial> lMaterial = std::make_unique<ComputeMaterial>();
	ID3D12Device* lDevice = adapter_->GetDevice()->Get();

	//頂点シェーダの読み込み
	lMaterial->computeShader = CreateUniqueShader("Resources/Shaders/AliceParticle/BloodGush/EmitCS.hlsl","main","cs_5_0",IShader::ShaderType::CS);

	//ルートシグネチャ設定
	lMaterial->rootSignature = CreateUniqueRootSignature();

	lMaterial->rootSignature->Add(IRootSignature::RootType::CBV,0);//b0
	lMaterial->rootSignature->Add(IRootSignature::RootType::CBV,1);//b1
	lMaterial->rootSignature->Add(IRootSignature::RootType::CBV,2);//b2

	lMaterial->rootSignature->Add(IRootSignature::RangeType::UAV,0);//u0
	lMaterial->rootSignature->Add(IRootSignature::RangeType::UAV,1);//u1
	lMaterial->rootSignature->Add(IRootSignature::RangeType::UAV,2);//u1

	lMaterial->rootSignature->Create(lDevice);

	//生成
	lMaterial->Initialize(adapter_->GetDevice());

	manager_->AddMaterial(lMaterial,"ComputeBloodGushGPUParticleEmit");
}

void CreateBloodGushUpdateComputeMaterial(MaterialManager* manager_,IAdapter* adapter_)
{
	std::unique_ptr<ComputeMaterial> lMaterial = std::make_unique<ComputeMaterial>();
	ID3D12Device* lDevice = adapter_->GetDevice()->Get();

	//頂点シェーダの読み込み
	lMaterial->computeShader = CreateUniqueShader("Resources/Shaders/AliceParticle/BloodGush/UpdateCS.hlsl","main","cs_5_0",IShader::ShaderType::CS);

	//ルートシグネチャ設定
	lMaterial->rootSignature = CreateUniqueRootSignature();

	lMaterial->rootSignature->Add(IRootSignature::RootType::CBV,0);//b0
	lMaterial->rootSignature->Add(IRootSignature::RootType::CBV,1);//b1
	lMaterial->rootSignature->Add(IRootSignature::RootType::CBV,2);//u2

	lMaterial->rootSignature->Add(IRootSignature::RangeType::UAV,0);//u0
	lMaterial->rootSignature->Add(IRootSignature::RangeType::UAV,1);//u1
	lMaterial->rootSignature->Add(IRootSignature::RangeType::UAV,2);//u2
	lMaterial->rootSignature->Add(IRootSignature::RangeType::UAV,3);//u2

	lMaterial->rootSignature->Create(lDevice);

	//生成
	lMaterial->Initialize(adapter_->GetDevice());

	manager_->AddMaterial(lMaterial,"ComputeBloodGushGPUParticleUpdate");
}

void CreateBloodGushDrawArgumentUpdateComputeMaterial(MaterialManager* manager_,IAdapter* adapter_)
{
	std::unique_ptr<ComputeMaterial> lMaterial = std::make_unique<ComputeMaterial>();
	ID3D12Device* lDevice = adapter_->GetDevice()->Get();

	//シェーダの読み込み
	lMaterial->computeShader = CreateUniqueShader("Resources/Shaders/AliceParticle/BloodGush/DrawArgumentUpdateCS.hlsl","main","cs_5_0",IShader::ShaderType::CS);

	//ルートシグネチャ設定
	lMaterial->rootSignature = CreateUniqueRootSignature();

	lMaterial->rootSignature->Add(IRootSignature::RangeType::UAV,0);//u0
	lMaterial->rootSignature->Add(IRootSignature::RangeType::UAV,1);//u1
	lMaterial->rootSignature->Add(IRootSignature::RangeType::UAV,2);//u1

	lMaterial->rootSignature->Create(lDevice);

	//生成
	lMaterial->Initialize(adapter_->GetDevice());

	manager_->AddMaterial(lMaterial,"ComputeBloodGushGPUParticleDrawArgumentUpdate");
}

void CreateBloodGushFreeListInitComputeMaterial(MaterialManager* manager_,IAdapter* adapter_)
{
	std::unique_ptr<ComputeMaterial> lMaterial = std::make_unique<ComputeMaterial>();
	ID3D12Device* lDevice = adapter_->GetDevice()->Get();

	//シェーダの読み込み
	lMaterial->computeShader = CreateUniqueShader("Resources/Shaders/AliceParticle/BloodGush/FreeListInitializeCS.hlsl","main","cs_5_0",IShader::ShaderType::CS);

	//ルートシグネチャ設定
	lMaterial->rootSignature = CreateUniqueRootSignature();

	lMaterial->rootSignature->Add(IRootSignature::RangeType::UAV,0);//u0

	lMaterial->rootSignature->Add(IRootSignature::RootType::CBV,0);//b0

	lMaterial->rootSignature->Create(lDevice);

	//生成
	lMaterial->Initialize(adapter_->GetDevice());

	manager_->AddMaterial(lMaterial,"ComputeBloodGushGPUParticleFreeListInit");
}
