#include "DefaultPostEffect.h"

void CreateDefaultPostEffectMaterial(MaterialManager* manager_,IAdapter* adapter_)
{
	std::unique_ptr<Material>lDefaultPostEffectMaterial = std::make_unique<Material>();
	ID3D12Device* lDevice = adapter_->GetDevice()->Get();

	//頂点シェーダの読み込み
	lDefaultPostEffectMaterial->vertexShader = CreateUniqueShader("Resources/Shaders/2D/PostEffect/PostEffectTest/PostEffectTestVS.hlsl");

	//ピクセルシェーダの読み込み
	lDefaultPostEffectMaterial->pixelShader = CreateUniqueShader("Resources/Shaders/2D/PostEffect/PostEffectTest/PostEffectTestPS.hlsl","main","ps_5_0");

	//頂点レイアウト設定
	lDefaultPostEffectMaterial->inputLayouts =
	{
		// xyz座標
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
		// uv座標
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
		//カラー
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
	};

	lDefaultPostEffectMaterial->depthFlag = false;

	lDefaultPostEffectMaterial->blenddesc.RenderTarget[ 0 ].BlendEnable = true;// ブレンドを有効
	lDefaultPostEffectMaterial->blenddesc.RenderTarget[ 0 ].SrcBlend = D3D12_BLEND_SRC_ALPHA;// ソースのアルファ値
	lDefaultPostEffectMaterial->blenddesc.RenderTarget[ 0 ].DestBlend = D3D12_BLEND_INV_SRC_ALPHA;// 1.0f-ソースのアルファ値

	//ルートシグネチャ設定
	lDefaultPostEffectMaterial->rootSignature = CreateUniqueRootSignature();
	lDefaultPostEffectMaterial->rootSignature->Add(IRootSignature::RangeType::SRV,0);//t0
	lDefaultPostEffectMaterial->rootSignature->AddStaticSampler(0);//s0
	lDefaultPostEffectMaterial->rootSignature->Create(lDevice);

	//生成
	lDefaultPostEffectMaterial->Initialize();

	manager_->AddMaterial(lDefaultPostEffectMaterial,"DefaultPostEffect");
}
