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
	std::unique_ptr<IRootSignature> rootSignature;

	//頂点シェーダ
	std::unique_ptr<IShader> vertexShader;
	//ピクセルシェーダ
	std::unique_ptr<IShader> pixelShader;
	//ジオメトリシェーダ
	std::unique_ptr<IShader> geometryShader;
	//ハルシェーダ
	std::unique_ptr<IShader> hullShader;
	//ドメインシェーダ
	std::unique_ptr<IShader> domainShader;

	RenderTargetFormat renderTargetFormat;

	D3D12_PRIMITIVE_TOPOLOGY_TYPE primitiveType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	D3D12_CULL_MODE cullMode = D3D12_CULL_MODE_BACK;

	//深度フラグ
	bool depthFlag = true;
	int8_t PADING[3]{};

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

