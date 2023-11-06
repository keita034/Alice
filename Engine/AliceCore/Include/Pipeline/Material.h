#pragma once
#include<Defined.h>

ALICE_SUPPRESS_WARNINGS_BEGIN

#include<vector>

ALICE_SUPPRESS_WARNINGS_END

#include<Shader.h>
#include<PipelineState.h>
#include<RootSignature.h>
#include<BaseMaterial.h>

class Material : public BaseMaterial
{
public:
	std::string name;

	//頂点レイアウト
	std::vector<D3D12_INPUT_ELEMENT_DESC> inputLayouts;

	//ブレンドステート
	D3D12_BLEND_DESC blenddesc;

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

	D3D12_DEPTH_WRITE_MASK depthWriteMask = D3D12_DEPTH_WRITE_MASK::D3D12_DEPTH_WRITE_MASK_ALL;

	//深度フラグ
	bool depthFlag = true;

	int8_t PADING[ 7 ];
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

