#include "IcosahedronParticle.h"

void CreateIcosahedronParticleMaterial(MaterialManager* manager_,IAdapter* adapter_)
{
	std::unique_ptr<Material>lDefaultParticleMaterial = std::make_unique<Material>();
	ID3D12Device* lDevice = adapter_->GetDevice()->Get();

	//頂点シェーダの読み込み
	lDefaultParticleMaterial->vertexShader = CreateUniqueShader("Resources/Shaders/2D/Particle/ParticleVS.hlsl");

	//ピクセルシェーダの読み込み
	lDefaultParticleMaterial->pixelShader = CreateUniqueShader("Resources/Shaders/2D/Particle/ParticlePS.hlsl","main","ps_5_0",IShader::ShaderType::PS);

	lDefaultParticleMaterial->geometryShader = CreateUniqueShader("Resources/Shaders/2D/Particle/IcosahedronGS.hlsl","main","gs_5_0",IShader::ShaderType::GS);

	//頂点レイアウト設定
	lDefaultParticleMaterial->inputLayouts = {
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
	lDefaultParticleMaterial->rootSignature = CreateUniqueRootSignature();
	lDefaultParticleMaterial->rootSignature->Add(IRootSignature::RootType::CBV,0);//b0
	lDefaultParticleMaterial->rootSignature->Add(IRootSignature::RangeType::SRV,0);//t0
	lDefaultParticleMaterial->rootSignature->AddStaticSampler(0);//s0
	lDefaultParticleMaterial->rootSignature->Create(lDevice);

	//ブレンド設定
	lDefaultParticleMaterial->blenddesc.RenderTarget[ 0 ].BlendEnable = true;// ブレンドを有効
	lDefaultParticleMaterial->blenddesc.RenderTarget[ 0 ].SrcBlend = D3D12_BLEND_SRC_ALPHA;// ソースのアルファ値
	lDefaultParticleMaterial->blenddesc.RenderTarget[ 0 ].DestBlend = D3D12_BLEND_INV_SRC_ALPHA;// 1.0f-ソースのアルファ値

	lDefaultParticleMaterial->primitiveType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT;

	lDefaultParticleMaterial->depthFlag = false;

	lDefaultParticleMaterial->cullMode = D3D12_CULL_MODE_NONE;

	//生成
	lDefaultParticleMaterial->Initialize();

	manager_->AddMaterial(lDefaultParticleMaterial,"IcosahedronParticle");
}
