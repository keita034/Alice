#include "ToonModel.h"

#include<CreateBlend.h>

void CreateToonModelMaterial(MaterialManager* manager_,IAdapter* adapter_)
{
	std::unique_ptr<Material>lDefaultModelMaterial = std::make_unique<Material>();
	ID3D12Device* lDevice = adapter_->GetDevice()->Get();

	//頂点シェーダの読み込み
	lDefaultModelMaterial->vertexShader = CreateUniqueShader("Resources/Shaders/AliceModel/ToonModel/ToonModelVS.hlsl");

	//ピクセルシェーダの読み込み
	lDefaultModelMaterial->pixelShader = CreateUniqueShader("Resources/Shaders/AliceModel/ToonModel/ToonModelPS.hlsl","main","ps_5_0",IShader::ShaderType::PS);

	//頂点レイアウト設定
	lDefaultModelMaterial->inputLayouts = {
	{ "POSITION",	0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0  }, // float3のPOSITION
	{ "NORMAL",		0, DXGI_FORMAT_R32G32B32_FLOAT,		0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0   }, // float3のNORMAL
	{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT,		0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0  }, // float2のTEXCOORD
	{ "TANGENT",	0, DXGI_FORMAT_R32G32B32_FLOAT,		0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }, // float3のTANGENT
	{ "COLOR",		0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }, // float4のCOLOR
	{ "INDEX",		0, DXGI_FORMAT_R32G32B32A32_UINT,	0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }, // UINT[4]のINDEX
	{ "WEIGHT",		0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }, // float[4]のWEIGHT
	};

	//ルートシグネチャ設定
	lDefaultModelMaterial->rootSignature = CreateUniqueRootSignature();
	lDefaultModelMaterial->rootSignature->Add(IRootSignature::RootType::CBV,0);//b0
	lDefaultModelMaterial->rootSignature->Add(IRootSignature::RootType::CBV,1);//b1
	lDefaultModelMaterial->rootSignature->Add(IRootSignature::RootType::CBV,2);//b2
	lDefaultModelMaterial->rootSignature->Add(IRootSignature::RootType::CBV,3);//b3
	lDefaultModelMaterial->rootSignature->Add(IRootSignature::RangeType::SRV,0);//t0
	lDefaultModelMaterial->rootSignature->AddStaticSampler(0);//s0
	lDefaultModelMaterial->rootSignature->Add(IRootSignature::RangeType::SRV,1);//t1
	lDefaultModelMaterial->rootSignature->AddStaticSampler(1);//s1
	lDefaultModelMaterial->rootSignature->Create(lDevice);


	lDefaultModelMaterial->blenddesc = CreateBlend(BlendMode::AX_BLENDMODE_ALPHA);

	lDefaultModelMaterial->cullMode = D3D12_CULL_MODE_BACK;
	//生成
	lDefaultModelMaterial->Initialize();

	manager_->AddMaterial(lDefaultModelMaterial,"DefaultToonModel");
}

void CreateToonModelAnimationMaterial(MaterialManager* manager_,IAdapter* adapter_)
{
	std::unique_ptr<Material>lDefaultModelMaterial = std::make_unique<Material>();
	ID3D12Device* lDevice = adapter_->GetDevice()->Get();

	//頂点シェーダの読み込み
	lDefaultModelMaterial->vertexShader = CreateUniqueShader("Resources/Shaders/AliceModel/ToonModel/ToonModelAnimationVS.hlsl");

	//ピクセルシェーダの読み込み
	lDefaultModelMaterial->pixelShader = CreateUniqueShader("Resources/Shaders/AliceModel/ToonModel/ToonModelPS.hlsl","main","ps_5_0",IShader::ShaderType::PS);

	//頂点レイアウト設定
	lDefaultModelMaterial->inputLayouts = {
	{ "POSITION",	0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0  }, // float3のPOSITION
	{ "NORMAL",		0, DXGI_FORMAT_R32G32B32_FLOAT,		0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0   }, // float3のNORMAL
	{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT,		0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0  }, // float2のTEXCOORD
	{ "TANGENT",	0, DXGI_FORMAT_R32G32B32_FLOAT,		0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }, // float3のTANGENT
	{ "COLOR",		0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }, // float4のCOLOR
	{ "INDEX",		0, DXGI_FORMAT_R32G32B32A32_UINT,	0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }, // UINT[4]のINDEX
	{ "WEIGHT",		0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }, // float[4]のWEIGHT
	};

	//ルートシグネチャ設定
	lDefaultModelMaterial->rootSignature = CreateUniqueRootSignature();
	lDefaultModelMaterial->rootSignature->Add(IRootSignature::RootType::CBV,0);//b0
	lDefaultModelMaterial->rootSignature->Add(IRootSignature::RootType::CBV,1);//b1
	lDefaultModelMaterial->rootSignature->Add(IRootSignature::RootType::CBV,2);//b2
	lDefaultModelMaterial->rootSignature->Add(IRootSignature::RootType::CBV,3);//b3
	lDefaultModelMaterial->rootSignature->Add(IRootSignature::RangeType::SRV,0);//t0
	lDefaultModelMaterial->rootSignature->AddStaticSampler(0);//s0
	lDefaultModelMaterial->rootSignature->Add(IRootSignature::RangeType::SRV,1);//t1
	lDefaultModelMaterial->rootSignature->AddStaticSampler(1);//s1
	lDefaultModelMaterial->rootSignature->Create(lDevice);


	lDefaultModelMaterial->blenddesc = CreateBlend(BlendMode::AX_BLENDMODE_ALPHA);

	lDefaultModelMaterial->cullMode = D3D12_CULL_MODE_BACK;
	//生成
	lDefaultModelMaterial->Initialize();

	manager_->AddMaterial(lDefaultModelMaterial,"DefaultToonModelAnimation");
}

void CreateToonModelOutLineAnimationMaterial(MaterialManager* manager_,IAdapter* adapter_)
{
	std::unique_ptr<Material>lDefaultModelMaterial = std::make_unique<Material>();
	ID3D12Device* lDevice = adapter_->GetDevice()->Get();

	//頂点シェーダの読み込み
	lDefaultModelMaterial->vertexShader = CreateUniqueShader("Resources/Shaders/AliceModel/ToonModel/ToonModelAnimationOutLineVS.hlsl");

	//ピクセルシェーダの読み込み
	lDefaultModelMaterial->pixelShader = CreateUniqueShader("Resources/Shaders/AliceModel/ToonModel/ToonModelOutLinePS.hlsl","main","ps_5_0",IShader::ShaderType::PS);

	//頂点レイアウト設定
	lDefaultModelMaterial->inputLayouts = {
	{ "POSITION",	0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0  }, // float3のPOSITION
	{ "NORMAL",		0, DXGI_FORMAT_R32G32B32_FLOAT,		0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0   }, // float3のNORMAL
	{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT,		0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0  }, // float2のTEXCOORD
	{ "TANGENT",	0, DXGI_FORMAT_R32G32B32_FLOAT,		0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }, // float3のTANGENT
	{ "COLOR",		0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }, // float4のCOLOR
	{ "INDEX",		0, DXGI_FORMAT_R32G32B32A32_UINT,	0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }, // UINT[4]のINDEX
	{ "WEIGHT",		0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }, // float[4]のWEIGHT
	};

	//ルートシグネチャ設定
	lDefaultModelMaterial->rootSignature = CreateUniqueRootSignature();
	lDefaultModelMaterial->rootSignature->Add(IRootSignature::RootType::CBV,0);//b0
	lDefaultModelMaterial->rootSignature->Add(IRootSignature::RootType::CBV,1);//b1
	lDefaultModelMaterial->rootSignature->Create(lDevice);


	lDefaultModelMaterial->blenddesc = CreateBlend(BlendMode::AX_BLENDMODE_ALPHA);

	lDefaultModelMaterial->cullMode = D3D12_CULL_MODE_FRONT;
	//生成
	lDefaultModelMaterial->Initialize();

	manager_->AddMaterial(lDefaultModelMaterial,"DefaultToonModelAnimationOutLine");
}

void CreateToonModelOutLineMaterial(MaterialManager* manager_,IAdapter* adapter_)
{
	std::unique_ptr<Material>lDefaultModelMaterial = std::make_unique<Material>();
	ID3D12Device* lDevice = adapter_->GetDevice()->Get();

	//頂点シェーダの読み込み
	lDefaultModelMaterial->vertexShader = CreateUniqueShader("Resources/Shaders/AliceModel/ToonModel/ToonModelOutLineVS.hlsl");

	//ピクセルシェーダの読み込み
	lDefaultModelMaterial->pixelShader = CreateUniqueShader("Resources/Shaders/AliceModel/ToonModel/ToonModelOutLinePS.hlsl","main","ps_5_0",IShader::ShaderType::PS);

	//頂点レイアウト設定
	lDefaultModelMaterial->inputLayouts = {
	{ "POSITION",	0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0  }, // float3のPOSITION
	{ "NORMAL",		0, DXGI_FORMAT_R32G32B32_FLOAT,		0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0   }, // float3のNORMAL
	{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT,		0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0  }, // float2のTEXCOORD
	{ "TANGENT",	0, DXGI_FORMAT_R32G32B32_FLOAT,		0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }, // float3のTANGENT
	{ "COLOR",		0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }, // float4のCOLOR
	{ "INDEX",		0, DXGI_FORMAT_R32G32B32A32_UINT,	0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }, // UINT[4]のINDEX
	{ "WEIGHT",		0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }, // float[4]のWEIGHT
	};

	//ルートシグネチャ設定
	lDefaultModelMaterial->rootSignature = CreateUniqueRootSignature();
	lDefaultModelMaterial->rootSignature->Add(IRootSignature::RootType::CBV,0);//b0
	lDefaultModelMaterial->rootSignature->Add(IRootSignature::RootType::CBV,1);//b1
	lDefaultModelMaterial->rootSignature->Create(lDevice);


	lDefaultModelMaterial->blenddesc = CreateBlend(BlendMode::AX_BLENDMODE_ALPHA);

	lDefaultModelMaterial->cullMode = D3D12_CULL_MODE_FRONT;
	//生成
	lDefaultModelMaterial->Initialize();

	manager_->AddMaterial(lDefaultModelMaterial,"DefaultToonModelOutLine");
}

void CreateZeldaToonModelMaterial(MaterialManager* manager_,IAdapter* adapter_)
{
	std::unique_ptr<Material>lDefaultModelMaterial = std::make_unique<Material>();
	ID3D12Device* lDevice = adapter_->GetDevice()->Get();

	//頂点シェーダの読み込み
	lDefaultModelMaterial->vertexShader = CreateUniqueShader("Resources/Shaders/AliceModel/ToonModel/ZeldaToonModelVS.hlsl");

	//ピクセルシェーダの読み込み
	lDefaultModelMaterial->pixelShader = CreateUniqueShader("Resources/Shaders/AliceModel/ToonModel/ZeldaToonModelPS.hlsl","main","ps_5_0",IShader::ShaderType::PS);

	//頂点レイアウト設定
	lDefaultModelMaterial->inputLayouts = {
	{ "POSITION",	0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0  }, // float3のPOSITION
	{ "NORMAL",		0, DXGI_FORMAT_R32G32B32_FLOAT,		0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0   }, // float3のNORMAL
	{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT,		0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0  }, // float2のTEXCOORD
	{ "TANGENT",	0, DXGI_FORMAT_R32G32B32_FLOAT,		0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }, // float3のTANGENT
	{ "COLOR",		0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }, // float4のCOLOR
	{ "INDEX",		0, DXGI_FORMAT_R32G32B32A32_UINT,	0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }, // UINT[4]のINDEX
	{ "WEIGHT",		0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }, // float[4]のWEIGHT
	};

	//ルートシグネチャ設定
	lDefaultModelMaterial->rootSignature = CreateUniqueRootSignature();
	lDefaultModelMaterial->rootSignature->Add(IRootSignature::RootType::CBV,0);//b0
	lDefaultModelMaterial->rootSignature->Add(IRootSignature::RootType::CBV,1);//b1
	lDefaultModelMaterial->rootSignature->Add(IRootSignature::RootType::CBV,2);//b2
	lDefaultModelMaterial->rootSignature->Add(IRootSignature::RootType::CBV,3);//b3
	lDefaultModelMaterial->rootSignature->Add(IRootSignature::RangeType::SRV,0);//t0
	lDefaultModelMaterial->rootSignature->AddStaticSampler(0);//s0
	lDefaultModelMaterial->rootSignature->Create(lDevice);


	lDefaultModelMaterial->blenddesc = CreateBlend(BlendMode::AX_BLENDMODE_ALPHA);

	lDefaultModelMaterial->cullMode = D3D12_CULL_MODE_BACK;
	lDefaultModelMaterial->depthFlag = true;

	//生成
	lDefaultModelMaterial->Initialize();

	manager_->AddMaterial(lDefaultModelMaterial,"DefaultZeldaToonModel");
}

void CreateZeldaToonModelAnimationMaterial(MaterialManager* manager_,IAdapter* adapter_)
{
	std::unique_ptr<Material>lDefaultModelMaterial = std::make_unique<Material>();
	ID3D12Device* lDevice = adapter_->GetDevice()->Get();

	//頂点シェーダの読み込み
	lDefaultModelMaterial->vertexShader = CreateUniqueShader("Resources/Shaders/AliceModel/ToonModel/ZeldaToonModelAnimationVS.hlsl");

	//ピクセルシェーダの読み込み
	lDefaultModelMaterial->pixelShader = CreateUniqueShader("Resources/Shaders/AliceModel/ToonModel/ZeldaToonModelPS.hlsl","main","ps_5_0",IShader::ShaderType::PS);

	//頂点レイアウト設定
	lDefaultModelMaterial->inputLayouts = {
	{ "POSITION",	0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0  }, // float3のPOSITION
	{ "NORMAL",		0, DXGI_FORMAT_R32G32B32_FLOAT,		0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0   }, // float3のNORMAL
	{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT,		0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0  }, // float2のTEXCOORD
	{ "TANGENT",	0, DXGI_FORMAT_R32G32B32_FLOAT,		0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }, // float3のTANGENT
	{ "COLOR",		0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }, // float4のCOLOR
	{ "INDEX",		0, DXGI_FORMAT_R32G32B32A32_UINT,	0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }, // UINT[4]のINDEX
	{ "WEIGHT",		0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }, // float[4]のWEIGHT
	};

	//ルートシグネチャ設定
	lDefaultModelMaterial->rootSignature = CreateUniqueRootSignature();
	lDefaultModelMaterial->rootSignature->Add(IRootSignature::RootType::CBV,0);//b0
	lDefaultModelMaterial->rootSignature->Add(IRootSignature::RootType::CBV,1);//b1
	lDefaultModelMaterial->rootSignature->Add(IRootSignature::RootType::CBV,2);//b2
	lDefaultModelMaterial->rootSignature->Add(IRootSignature::RootType::CBV,3);//b3
	lDefaultModelMaterial->rootSignature->Add(IRootSignature::RangeType::SRV,0);//t0
	lDefaultModelMaterial->rootSignature->AddStaticSampler(0);//s0
	lDefaultModelMaterial->rootSignature->Create(lDevice);

	lDefaultModelMaterial->blenddesc = CreateBlend(BlendMode::AX_BLENDMODE_ALPHA);

	lDefaultModelMaterial->cullMode = D3D12_CULL_MODE_BACK;
	//生成
	lDefaultModelMaterial->Initialize();

	manager_->AddMaterial(lDefaultModelMaterial,"DefaultZeldaToonModelAnimation");
}
