#include "TriangleSub.h"

ALICE_SUPPRESS_WARNINGS_BEGIN

#include<memory>

ALICE_SUPPRESS_WARNINGS_END

#include<CreateBlend.h>

void CreateTriangleSubMaterial(MaterialManager* manager_,IAdapter* adapter_)
{
	std::unique_ptr<Material> lMaterial = std::make_unique<Material>();
	ID3D12Device* lDevice = adapter_->GetDevice()->Get();

	//頂点シェーダの読み込み
	lMaterial->vertexShader = CreateUniqueShader("Resources/Shaders/2D/Mesh/MeshVS.hlsl");

	//ピクセルシェーダの読み込み
	lMaterial->pixelShader = CreateUniqueShader("Resources/Shaders/2D/Mesh/MeshPS.hlsl","main","ps_5_0",IShader::ShaderType::PS);

	//頂点レイアウト設定
	lMaterial->inputLayouts = {
		//座標
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
		//uv座標 
		{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
	};

	lMaterial->depthFlag = false;

	lMaterial->primitiveType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	lMaterial->blenddesc = CreateBlend(BlendMode::AX_BLENDMODE_SUB);

	//ルートシグネチャ設定
	lMaterial->rootSignature = CreateUniqueRootSignature();
	lMaterial->rootSignature->Add(IRootSignature::RootType::CBV,0);//b0
	lMaterial->rootSignature->Create(lDevice);
	//生成
	lMaterial->Initialize();

	manager_->AddMaterial(lMaterial,"MashTriangleSub");
}
