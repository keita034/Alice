#pragma once
#include<Defined.h>

ALICE_SUPPRESS_WARNINGS_BEGIN

#include<wrl.h>
#include<array>

ALICE_SUPPRESS_WARNINGS_END

#include<Shader.h>
#include<BasePipelineState.h>

struct RenderTargetFormat
{
	//描画対象数
	uint32_t NumRenderTargets;
	//描画対象のフォーマット
	std::array<DXGI_FORMAT, 8> RTVFormats;

	RenderTargetFormat();
};

/// <summary>
/// パイプラインステート
/// </summary>
class PipelineState : public BasePipelineState
{
private:
	//パイプラインステート
	Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelineState;
	//ルートシグネチャ
	ID3D12RootSignature* rootSignature = nullptr;
	//頂点シェーダーオブジェクト
	D3D12_SHADER_BYTECODE* vsByte = nullptr;
	//ピクセルシェーダーオブジェクト
	D3D12_SHADER_BYTECODE* psByte = nullptr;
	//ジオメトリシェーダーオブジェクト
	D3D12_SHADER_BYTECODE* gsByte = nullptr;
	//ハルシェーダーオブジェクト
	D3D12_SHADER_BYTECODE* hsByte = nullptr;
	//ドメインシェーダオブジェクト
	D3D12_SHADER_BYTECODE* dsByte = nullptr;

	//インプットレイアウトデータ
	D3D12_INPUT_ELEMENT_DESC* inputLayoutData;
	//インプットレイアウトサイズ
	size_t inputLayoutlength_;
	//ブレンドデスク
	D3D12_BLEND_DESC blendDesc = {};
	
	//プリミティブ形状
	D3D12_PRIMITIVE_TOPOLOGY_TYPE primitiveType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	
	//カリングのモード
	D3D12_CULL_MODE cullMode = D3D12_CULL_MODE_BACK;

	D3D12_FILL_MODE fillMode = D3D12_FILL_MODE_SOLID;

	//レンダーターゲットのフォーマット
	RenderTargetFormat renderTargetFormat;

	//深度フラグ
	bool depthFlag = true;
	int8_t PADING[7]{};

public:

	/// <summary>
	/// インプットレイアウトをセット
	/// </summary>
	/// <param name="desc">インプットレイアウトの先頭アドレス</param>
	/// <param name="length_">要素数</param>
	void SetInputLayout(D3D12_INPUT_ELEMENT_DESC* desc_, size_t length_);

	/// <summary>
	/// ルートシグネチャのアドレスをセット
	/// </summary>
	/// <param name="pRootSignature">ルートシグネチャのアドレス</param>
	void SetRootSignature(ID3D12RootSignature* pRootSignature_);

	/// <summary>
	/// ブレンドをセット
	/// </summary>
	/// <param name="desc">ブレンドデスク</param>
	void SetBlend(const D3D12_BLEND_DESC& desc_);

	/// <summary>
	/// 深度テストのフラグをセット
	/// </summary>
	/// <param name="flag">深度テストのフラグ</param>
	void SetDepthFlag(bool flag_);

	/// <summary>
	/// 頂点シェーダをセット
	/// </summary>
	/// <param name="vsByte">頂点シェーダオブジェクト</param>
	void SetVertexShader(D3D12_SHADER_BYTECODE* pVsByte_);

	/// <summary>
	/// ピクセルシェーダをセット
	/// </summary>
	/// <param name="psByte">ピクセルシェーダオブジェクト</param>
	void SetPixelShader(D3D12_SHADER_BYTECODE* pPsByte_);

	/// <summary>
	/// ジオメトリシェーダをセット
	/// </summary>
	/// <param name="gsByte">ジオメトリシェーダオブジェクト</param>
	void SetGeometryShader(D3D12_SHADER_BYTECODE* pGsByte_);

	/// <summary>
	/// ハルシェーダをセット
	/// </summary>
	/// <param name="hsByte">ハルシェーダオブジェクト</param>
	void SetHullShader(D3D12_SHADER_BYTECODE* pHsByte_);

	/// <summary>
	/// ドメインシェーダをセット
	/// </summary>
	/// <param name="pDsByte">ドメインシェーダオブジェクト</param>
	void SetDomainShader(D3D12_SHADER_BYTECODE* pDsByte_);

	/// <summary>
	/// プリミティブ形状をセット
	/// </summary>
	/// <param name="type">プリミティブ形状</param>
	void SetPrimitiveType(D3D12_PRIMITIVE_TOPOLOGY_TYPE type_);

	/// <summary>
	/// カリングモードをセット
	/// </summary>
	/// <param name="mode">モード</param>
	void SetCullMode(D3D12_CULL_MODE mode_);

	/// <summary>
	/// フィルモードをセット
	/// </summary>
	/// <param name="mode">モード</param>
	void SetFillMode(D3D12_FILL_MODE mode_);

	void SetRenderTargetFormat(const RenderTargetFormat& format_);

	/// <summary>
	/// パイプラインステートを生成
	/// </summary>
	/// <returns>生成出来たか</returns>
	bool Create();

	/// <summary>
	/// パイプラインステートを取得
	/// </summary>
	ID3D12PipelineState* GetPipelineState()const;
};

