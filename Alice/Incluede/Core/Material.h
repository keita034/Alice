#pragma once
#include<AliceMathUtility.h>
#include<DirectX12Core.h>
#include<AliceUtility.h>
#include<Shader.h>
#include<PipelineState.h>
#include<RootSignature.h>
#include<TextureManager.h>

class Material
{
public:

	std::string name;

	//テクスチャデータ
	TextureData* textureData;

	//頂点レイアウト
	std::vector<D3D12_INPUT_ELEMENT_DESC> inputLayouts;

	//ブレンドステート
	D3D12_BLEND_DESC blenddesc = CD3DX12_BLEND_DESC(D3D12_DEFAULT);

	//パイプラインステート
	std::unique_ptr<PipelineState> pipelineState;

	//ルートシグネチャ
	std::unique_ptr<RootSignature> rootSignature;

	//頂点シェーダ
	std::unique_ptr<Shader> vertexShader;
	//ピクセルシェーダ
	std::unique_ptr<Shader> pixelShader;
	//ジオメトリシェーダ
	std::unique_ptr<Shader> geometryShader;
	//ハルシェーダ
	std::unique_ptr<Shader> hullShader;
	//ドメインシェーダ
	std::unique_ptr<Shader> domainShader;

	//深度フラグ
	bool depthFlag = true;
	char PADING[3]{};

	D3D12_PRIMITIVE_TOPOLOGY_TYPE primitiveType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	D3D12_CULL_MODE cullMode = D3D12_CULL_MODE_BACK;
	char PADING2[4]{};

public:

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	Material() = default;
	~Material();

private:
	//コピーコンストラクタ・代入演算子削除
	Material(const Material&) = delete;
	Material& operator=(const Material&) = delete;
};

