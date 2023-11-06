#include "RainParticle.h"

void CreateRainParticleMaterial(MaterialManager* manager_,IAdapter* adapter_)
{
	std::unique_ptr<Material>lDefaultRainParticleMaterial = std::make_unique<Material>();
	ID3D12Device* lDevice = adapter_->GetDevice()->Get();

	//頂点シェーダの読み込み
	lDefaultRainParticleMaterial->vertexShader = CreateUniqueShader("Resources/Shaders/2D/Particle/RainParticleVS.hlsl");

	//ピクセルシェーダの読み込み
	lDefaultRainParticleMaterial->pixelShader = CreateUniqueShader("Resources/Shaders/2D/Particle/RainParticlePS.hlsl","main","ps_5_0",IShader::ShaderType::PS);

	lDefaultRainParticleMaterial->geometryShader = CreateUniqueShader("Resources/Shaders/2D/Particle/RainParticleGS.hlsl","main","gs_5_0",IShader::ShaderType::GS);

	//頂点レイアウト設定
	lDefaultRainParticleMaterial->inputLayouts = {
		// xyz座標
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
		//カラー
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
		//スケール
		{ "SCALE", 0, DXGI_FORMAT_R32_FLOAT, 0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
		//回転角
		{ "ROTATION", 0, DXGI_FORMAT_R32_FLOAT, 0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},

	};

	//ルートシグネチャ設定
	lDefaultRainParticleMaterial->rootSignature = CreateUniqueRootSignature();
	lDefaultRainParticleMaterial->rootSignature->Add(IRootSignature::RootType::CBV,0);//b0
	lDefaultRainParticleMaterial->rootSignature->Create(lDevice);

	//ブレンド設定
	lDefaultRainParticleMaterial->blenddesc.RenderTarget[ 0 ].BlendEnable = true;// ブレンドを有効
	lDefaultRainParticleMaterial->blenddesc.RenderTarget[ 0 ].SrcBlend = D3D12_BLEND_SRC_ALPHA;// ソースのアルファ値
	lDefaultRainParticleMaterial->blenddesc.RenderTarget[ 0 ].DestBlend = D3D12_BLEND_INV_SRC_ALPHA;// 1.0f-ソースのアルファ値

	lDefaultRainParticleMaterial->primitiveType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT;

	lDefaultRainParticleMaterial->depthFlag = false;

	//生成
	lDefaultRainParticleMaterial->Initialize();

	manager_->AddMaterial(lDefaultRainParticleMaterial,"DefaultRainParticle");
}
