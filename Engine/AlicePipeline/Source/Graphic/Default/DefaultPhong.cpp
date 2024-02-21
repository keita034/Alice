#include "DefaultPhong.h"

#include<CreateBlend.h>

void CreateDefaultPhongMaterial(MaterialManager* manager_,IAdapter* adapter_)
{
	std::unique_ptr<Material>lDefaultPhongMaterial = std::make_unique<Material>();

	ID3D12Device* lDevice = adapter_->GetDevice()->Get();

	//頂点シェーダの読み込み
	lDefaultPhongMaterial->vertexShader = CreateUniqueShader("Resources/Shaders/AliceModel/Phong/PhongVS.hlsl");

	//ピクセルシェーダの読み込み
	lDefaultPhongMaterial->pixelShader = CreateUniqueShader("Resources/Shaders/AliceModel/Phong/PhongPS.hlsl","main","ps_5_0",IShader::ShaderType::PS);

	//頂点レイアウト設定
	lDefaultPhongMaterial->inputLayouts = {
	{ "POSITION",	0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0  }, // float3のPOSITION
	{ "NORMAL",		0, DXGI_FORMAT_R32G32B32_FLOAT,		0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0   }, // float3のNORMAL
	{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT,		0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0  }, // float2のTEXCOORD
	{ "TANGENT",	0, DXGI_FORMAT_R32G32B32_FLOAT,		0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }, // float3のTANGENT
	{ "COLOR",		0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }, // float4のCOLOR
	{ "INDEX",		0, DXGI_FORMAT_R32G32B32A32_UINT,	0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }, // UINT[4]のINDEX
	{ "WEIGHT",		0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }, // float[4]のWEIGHT
	};

	//ルートシグネチャ設定
	lDefaultPhongMaterial->rootSignature = CreateUniqueRootSignature();
	lDefaultPhongMaterial->rootSignature->Add(IRootSignature::RootType::CBV,0);//b0
	lDefaultPhongMaterial->rootSignature->Add(IRootSignature::RootType::CBV,1);//b1
	lDefaultPhongMaterial->rootSignature->Add(IRootSignature::RootType::CBV,2);//b2
	lDefaultPhongMaterial->rootSignature->Add(IRootSignature::RootType::CBV,3);//b3
	lDefaultPhongMaterial->rootSignature->Add(IRootSignature::RangeType::SRV,0);//t0
	lDefaultPhongMaterial->rootSignature->AddStaticSampler(0);//s0
	lDefaultPhongMaterial->rootSignature->Create(lDevice);


	lDefaultPhongMaterial->blenddesc = CreateBlend(BlendMode::AX_BLENDMODE_ALPHA);

	lDefaultPhongMaterial->cullMode = D3D12_CULL_MODE_BACK;
	lDefaultPhongMaterial->depthFlag = true;

	//生成
	lDefaultPhongMaterial->Initialize();

	manager_->AddMaterial(lDefaultPhongMaterial,"DefaultPhong");
}
