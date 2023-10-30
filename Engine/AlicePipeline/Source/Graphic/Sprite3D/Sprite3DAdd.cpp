#include "Sprite3DAdd.h"

#include<CreateBlend.h>

void CreateSprite3DAddMaterial(MaterialManager* manager_,IAdapter* adapter_)
{
	std::unique_ptr<Material>lMaterial = std::make_unique<Material>();
	ID3D12Device* lDevice = adapter_->GetDevice()->Get();

	//頂点シェーダの読み込み
	lMaterial->vertexShader = CreateUniqueShader("Resources/Shaders/2D/Sprite/SpriteVS.hlsl");

	//ピクセルシェーダの読み込み
	lMaterial->pixelShader = CreateUniqueShader("Resources/Shaders/2D/Sprite/SpritePS.hlsl","main","ps_5_0",IShader::ShaderType::PS);


	//頂点レイアウト設定
	lMaterial->inputLayouts =
	{
		// xyz座標
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
		// uv座標
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
		//カラー
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
	};

	lMaterial->blenddesc.AlphaToCoverageEnable = true;

	lMaterial->blenddesc = CreateBlend(BlendMode::AX_BLENDMODE_ADD);

	//ルートシグネチャ設定
	lMaterial->rootSignature = CreateUniqueRootSignature();
	lMaterial->rootSignature->Add(IRootSignature::RootType::CBV,0);//b0
	lMaterial->rootSignature->Add(IRootSignature::RangeType::SRV,0);//t0
	lMaterial->rootSignature->AddStaticSampler(0);//s0
	lMaterial->rootSignature->Create(lDevice);

	//生成
	lMaterial->Initialize();
	manager_->AddMaterial(lMaterial,"Sprite3DAdd");
}
