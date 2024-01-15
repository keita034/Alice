#include "Fire.h"

#include<CreateBlend.h>
#include "Mesh.h"


void CreateAnimationMeshDrawMaterial(MaterialManager* manager_,IAdapter* adapter_)
{
	std::unique_ptr<Material>lMaterial = std::make_unique<Material>();
	ID3D12Device* lDevice = adapter_->GetDevice()->Get();

	//頂点シェーダの読み込み
	lMaterial->vertexShader = CreateUniqueShader("Resources/Shaders/AliceParticle/AnimationMesh/VS.hlsl");

	//ジオメトリシェーダの読み込み
	lMaterial->geometryShader = CreateUniqueShader("Resources/Shaders/AliceParticle/AnimationMesh/GS.hlsl","main","gs_5_0",IShader::ShaderType::GS);

	//ピクセルシェーダの読み込み
	lMaterial->pixelShader = CreateUniqueShader("Resources/Shaders/AliceParticle/AnimationMesh/PS.hlsl","main","ps_5_0",IShader::ShaderType::PS);

	//ルートシグネチャ設定
	lMaterial->rootSignature = CreateUniqueRootSignature();
	lMaterial->rootSignature->Add(IRootSignature::RootType::CBV,0);//b0

	lMaterial->rootSignature->Add(IRootSignature::RangeType::UAV,0);//u0
	lMaterial->rootSignature->Add(IRootSignature::RangeType::UAV,1);//u1

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

	manager_->AddMaterial(lMaterial,"AnimationMeshGPUParticleDraw");
}

void CreateAnimationMeshEmitComputeMaterial(MaterialManager* manager_,IAdapter* adapter_)
{
	std::unique_ptr<ComputeMaterial> lMaterial = std::make_unique<ComputeMaterial>();
	ID3D12Device* lDevice = adapter_->GetDevice()->Get();

	//頂点シェーダの読み込み
	lMaterial->computeShader = CreateUniqueShader("Resources/Shaders/AliceParticle/AnimationMesh/EmitCS.hlsl","main","cs_5_0",IShader::ShaderType::CS);

	//ルートシグネチャ設定
	lMaterial->rootSignature = CreateUniqueRootSignature();

	lMaterial->rootSignature->Add(IRootSignature::RootType::CBV,0);//b0
	lMaterial->rootSignature->Add(IRootSignature::RootType::CBV,1);//b1
	lMaterial->rootSignature->Add(IRootSignature::RootType::CBV,2);//b2
	lMaterial->rootSignature->Add(IRootSignature::RootType::CBV,3);//b3

	lMaterial->rootSignature->Add(IRootSignature::RangeType::UAV,0);//u0
	lMaterial->rootSignature->Add(IRootSignature::RangeType::UAV,1);//u1

	lMaterial->rootSignature->Add(IRootSignature::RangeType::SRV,0);//t0
	lMaterial->rootSignature->Add(IRootSignature::RangeType::SRV,1);//t1
	lMaterial->rootSignature->Add(IRootSignature::RangeType::SRV,2);//t1

	lMaterial->rootSignature->Create(lDevice);

	//生成
	lMaterial->Initialize(adapter_->GetDevice());

	manager_->AddMaterial(lMaterial,"ComputeAnimationMeshGPUParticleEmit");
}

void CreateAnimationMeshFreeListInitComputeMaterial(MaterialManager* manager_,IAdapter* adapter_)
{
	std::unique_ptr<ComputeMaterial> lMaterial = std::make_unique<ComputeMaterial>();
	ID3D12Device* lDevice = adapter_->GetDevice()->Get();

	//シェーダの読み込み
	lMaterial->computeShader = CreateUniqueShader("Resources/Shaders/AliceParticle/AnimationMesh/FreeListInitializeCS.hlsl","main","cs_5_0",IShader::ShaderType::CS);

	//ルートシグネチャ設定
	lMaterial->rootSignature = CreateUniqueRootSignature();

	lMaterial->rootSignature->Add(IRootSignature::RangeType::UAV,0);//u0

	lMaterial->rootSignature->Add(IRootSignature::RootType::CBV,0);//b0

	lMaterial->rootSignature->Create(lDevice);

	//生成
	lMaterial->Initialize(adapter_->GetDevice());

	manager_->AddMaterial(lMaterial,"ComputeAnimationMeshGPUParticleFreeListInit");
}

void CreateAnimationMeshUpdateComputeMaterial(MaterialManager* manager_,IAdapter* adapter_)
{
	std::unique_ptr<ComputeMaterial> lMaterial = std::make_unique<ComputeMaterial>();
	ID3D12Device* lDevice = adapter_->GetDevice()->Get();

	//頂点シェーダの読み込み
	lMaterial->computeShader = CreateUniqueShader("Resources/Shaders/AliceParticle/AnimationMesh/UpdateCS.hlsl","main","cs_5_0",IShader::ShaderType::CS);

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

	manager_->AddMaterial(lMaterial,"ComputeAnimationMeshGPUParticleUpdate");
}

void CreateAnimationMeshDrawArgumentUpdateComputeMaterial(MaterialManager* manager_,IAdapter* adapter_)
{
	std::unique_ptr<ComputeMaterial> lMaterial = std::make_unique<ComputeMaterial>();
	ID3D12Device* lDevice = adapter_->GetDevice()->Get();

	//シェーダの読み込み
	lMaterial->computeShader = CreateUniqueShader("Resources/Shaders/AliceParticle/AnimationMesh/DrawArgumentUpdateCS.hlsl","main","cs_5_0",IShader::ShaderType::CS);

	//ルートシグネチャ設定
	lMaterial->rootSignature = CreateUniqueRootSignature();

	lMaterial->rootSignature->Add(IRootSignature::RangeType::UAV,0);//u0
	lMaterial->rootSignature->Add(IRootSignature::RangeType::UAV,1);//u1
	lMaterial->rootSignature->Add(IRootSignature::RangeType::UAV,2);//u1

	lMaterial->rootSignature->Create(lDevice);

	//生成
	lMaterial->Initialize(adapter_->GetDevice());

	manager_->AddMaterial(lMaterial,"ComputeAnimationMeshGPUParticleDrawArgumentUpdate");
}



void CreateMeshDrawMaterial(MaterialManager* manager_,IAdapter* adapter_)
{
	std::unique_ptr<Material>lMaterial = std::make_unique<Material>();
	ID3D12Device* lDevice = adapter_->GetDevice()->Get();

	//頂点シェーダの読み込み
	lMaterial->vertexShader = CreateUniqueShader("Resources/Shaders/AliceParticle/Mesh/VS.hlsl");

	//ジオメトリシェーダの読み込み
	lMaterial->geometryShader = CreateUniqueShader("Resources/Shaders/AliceParticle/Mesh/GS.hlsl","main","gs_5_0",IShader::ShaderType::GS);

	//ピクセルシェーダの読み込み
	lMaterial->pixelShader = CreateUniqueShader("Resources/Shaders/AliceParticle/Mesh/PS.hlsl","main","ps_5_0",IShader::ShaderType::PS);

	//ルートシグネチャ設定
	lMaterial->rootSignature = CreateUniqueRootSignature();
	lMaterial->rootSignature->Add(IRootSignature::RootType::CBV,0);//b0

	lMaterial->rootSignature->Add(IRootSignature::RangeType::UAV,0);//u0
	lMaterial->rootSignature->Add(IRootSignature::RangeType::UAV,1);//u1

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

	manager_->AddMaterial(lMaterial,"MeshGPUParticleDraw");
}

void CreateMeshEmitComputeMaterial(MaterialManager* manager_,IAdapter* adapter_)
{
	std::unique_ptr<ComputeMaterial> lMaterial = std::make_unique<ComputeMaterial>();
	ID3D12Device* lDevice = adapter_->GetDevice()->Get();

	//頂点シェーダの読み込み
	lMaterial->computeShader = CreateUniqueShader("Resources/Shaders/AliceParticle/Mesh/EmitCS.hlsl","main","cs_5_0",IShader::ShaderType::CS);

	//ルートシグネチャ設定
	lMaterial->rootSignature = CreateUniqueRootSignature();

	lMaterial->rootSignature->Add(IRootSignature::RootType::CBV,0);//b0
	lMaterial->rootSignature->Add(IRootSignature::RootType::CBV,1);//b1
	lMaterial->rootSignature->Add(IRootSignature::RootType::CBV,2);//b2
	lMaterial->rootSignature->Add(IRootSignature::RootType::CBV,3);//b3

	lMaterial->rootSignature->Add(IRootSignature::RangeType::UAV,0);//u0
	lMaterial->rootSignature->Add(IRootSignature::RangeType::UAV,1);//u1

	lMaterial->rootSignature->Add(IRootSignature::RangeType::SRV,0);//t0
	lMaterial->rootSignature->Add(IRootSignature::RangeType::SRV,1);//t1
	//lMaterial->rootSignature->Add(IRootSignature::RangeType::SRV,2);//t1

	lMaterial->rootSignature->Create(lDevice);

	//生成
	lMaterial->Initialize(adapter_->GetDevice());

	manager_->AddMaterial(lMaterial,"ComputeMeshGPUParticleEmit");
}

void CreateMeshFreeListInitComputeMaterial(MaterialManager* manager_,IAdapter* adapter_)
{
	std::unique_ptr<ComputeMaterial> lMaterial = std::make_unique<ComputeMaterial>();
	ID3D12Device* lDevice = adapter_->GetDevice()->Get();

	//シェーダの読み込み
	lMaterial->computeShader = CreateUniqueShader("Resources/Shaders/AliceParticle/Mesh/FreeListInitializeCS.hlsl","main","cs_5_0",IShader::ShaderType::CS);

	//ルートシグネチャ設定
	lMaterial->rootSignature = CreateUniqueRootSignature();

	lMaterial->rootSignature->Add(IRootSignature::RangeType::UAV,0);//u0

	lMaterial->rootSignature->Add(IRootSignature::RootType::CBV,0);//b0

	lMaterial->rootSignature->Create(lDevice);

	//生成
	lMaterial->Initialize(adapter_->GetDevice());

	manager_->AddMaterial(lMaterial,"ComputeMeshGPUParticleFreeListInit");
}

void CreateMeshUpdateComputeMaterial(MaterialManager* manager_,IAdapter* adapter_)
{
	std::unique_ptr<ComputeMaterial> lMaterial = std::make_unique<ComputeMaterial>();
	ID3D12Device* lDevice = adapter_->GetDevice()->Get();

	//頂点シェーダの読み込み
	lMaterial->computeShader = CreateUniqueShader("Resources/Shaders/AliceParticle/Mesh/UpdateCS.hlsl","main","cs_5_0",IShader::ShaderType::CS);

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

	manager_->AddMaterial(lMaterial,"ComputeMeshGPUParticleUpdate");
}

void CreateMeshDrawArgumentUpdateComputeMaterial(MaterialManager* manager_,IAdapter* adapter_)
{
	std::unique_ptr<ComputeMaterial> lMaterial = std::make_unique<ComputeMaterial>();
	ID3D12Device* lDevice = adapter_->GetDevice()->Get();

	//シェーダの読み込み
	lMaterial->computeShader = CreateUniqueShader("Resources/Shaders/AliceParticle/Mesh/DrawArgumentUpdateCS.hlsl","main","cs_5_0",IShader::ShaderType::CS);

	//ルートシグネチャ設定
	lMaterial->rootSignature = CreateUniqueRootSignature();

	lMaterial->rootSignature->Add(IRootSignature::RangeType::UAV,0);//u0
	lMaterial->rootSignature->Add(IRootSignature::RangeType::UAV,1);//u1
	lMaterial->rootSignature->Add(IRootSignature::RangeType::UAV,2);//u1

	lMaterial->rootSignature->Create(lDevice);

	//生成
	lMaterial->Initialize(adapter_->GetDevice());

	manager_->AddMaterial(lMaterial,"ComputeMeshGPUParticleDrawArgumentUpdate");
}

