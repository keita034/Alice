#include "DefaultLambert.h"

void CreateDefaultLambertMaterial(MaterialManager* manager_,IAdapter* adapter_)
{
	std::unique_ptr<Material>lMaterial = std::make_unique<Material>();

	ID3D12Device* lDevice = adapter_->GetDevice()->Get();

	//頂点シェーダの読み込み
	lMaterial->vertexShader = CreateUniqueShader("Resources/Shaders/3D/Model/Lambert/LambertVS.hlsl");

	//ピクセルシェーダの読み込み
	lMaterial->pixelShader = CreateUniqueShader("Resources/Shaders/3D/Model/Lambert/LambertPS.hlsl","main","ps_5_0",IShader::ShaderType::PS);

	//頂点レイアウト設定
	lMaterial->inputLayouts = {
		// xyz座標
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
		//法線
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
		// uv座標
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
	};

	//ルートシグネチャ設定
	lMaterial->rootSignature = CreateUniqueRootSignature();
	lMaterial->rootSignature->Add(IRootSignature::RootType::CBV,0);//b0
	lMaterial->rootSignature->Add(IRootSignature::RootType::CBV,1);//b1
	lMaterial->rootSignature->Add(IRootSignature::RootType::CBV,2);//b2
	lMaterial->rootSignature->Add(IRootSignature::RootType::CBV,3);//b3
	lMaterial->rootSignature->Add(IRootSignature::RangeType::SRV,0);//t0
	lMaterial->rootSignature->AddStaticSampler(0);//s0
	lMaterial->rootSignature->Create(lDevice);

	//ブレンド設定
	lMaterial->blenddesc.RenderTarget[ 0 ].BlendEnable = true;// ブレンドを有効
	lMaterial->blenddesc.RenderTarget[ 0 ].SrcBlend = D3D12_BLEND_SRC_ALPHA;// ソースのアルファ値
	lMaterial->blenddesc.RenderTarget[ 0 ].DestBlend = D3D12_BLEND_INV_SRC_ALPHA;// 1.0f-ソースのアルファ値

	//生成
	lMaterial->Initialize();

	manager_->AddMaterial(lMaterial,"DefaultLambert");
}
