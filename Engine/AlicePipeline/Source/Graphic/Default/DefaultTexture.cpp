#include "DefaultTexture.h"

void CreateDefaultTextureMaterial(MaterialManager* manager_,IAdapter* adapter_)
{
	std::unique_ptr<Material> lDefaultTextureMaterial = std::make_unique<Material>();

	ID3D12Device* lDevice = adapter_->GetDevice()->Get();

	//頂点シェーダの読み込み
	lDefaultTextureMaterial->vertexShader = CreateUniqueShader("Resources/Shaders/3D/Model/ModelBasic/ModelBasicVS.hlsl");

	//ジオメトリシェーダの読み込み
	lDefaultTextureMaterial->geometryShader = CreateUniqueShader("Resources/Shaders/3D/Model/ModelBasic/PolygonEffectGS.hlsl","main","gs_5_0",IShader::ShaderType::GS);

	//ピクセルシェーダの読み込み
	lDefaultTextureMaterial->pixelShader = CreateUniqueShader("Resources/Shaders/3D/Model/ModelBasic/ModelBasicPS.hlsl","main","ps_5_0",IShader::ShaderType::PS);

	//頂点レイアウト設定
	lDefaultTextureMaterial->inputLayouts = {
		// xyz座標
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
		//法線
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
		// uv座標
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
	};

	//ルートシグネチャ設定
	lDefaultTextureMaterial->rootSignature = CreateUniqueRootSignature();
	lDefaultTextureMaterial->rootSignature->Add(IRootSignature::RootType::CBV,0);//b0
	lDefaultTextureMaterial->rootSignature->Add(IRootSignature::RootType::CBV,1);//b1
	lDefaultTextureMaterial->rootSignature->Add(IRootSignature::RootType::CBV,2);//b2
	lDefaultTextureMaterial->rootSignature->Add(IRootSignature::RootType::CBV,3);//b3
	lDefaultTextureMaterial->rootSignature->Add(IRootSignature::RangeType::SRV,0);//t0
	lDefaultTextureMaterial->rootSignature->AddStaticSampler(0);//s0
	lDefaultTextureMaterial->rootSignature->Create(lDevice);

	//ブレンド設定
	lDefaultTextureMaterial->blenddesc.RenderTarget[ 0 ].BlendEnable = true;// ブレンドを有効
	lDefaultTextureMaterial->blenddesc.RenderTarget[ 0 ].SrcBlend = D3D12_BLEND_SRC_ALPHA;// ソースのアルファ値
	lDefaultTextureMaterial->blenddesc.RenderTarget[ 0 ].DestBlend = D3D12_BLEND_INV_SRC_ALPHA;// 1.0f-ソースのアルファ値

	lDefaultTextureMaterial->cullMode = D3D12_CULL_MODE_NONE;

	//生成
	lDefaultTextureMaterial->Initialize();

	manager_->AddMaterial(lDefaultTextureMaterial,"DefaultTexture");
}
