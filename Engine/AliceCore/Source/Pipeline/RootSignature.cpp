#include<RootSignature.h>

#pragma warning(push)
#pragma warning(disable: 4061)
#pragma warning(disable: 4062)
#pragma warning(disable: 4265)
#pragma warning(disable: 4365)
#pragma warning(disable: 4514)
#pragma warning(disable: 4625)
#pragma warning(disable: 4626)
#pragma warning(disable: 4668)
#pragma warning(disable: 4710)
#pragma warning(disable: 4820)
#pragma warning(disable: 5039)
#pragma warning(disable: 5204)
#pragma warning(disable: 5220)

#include<wrl.h>
#include<vector>
#include<directx/d3dx12.h>
#include<cassert>

#pragma warning(pop)

/// <summary>
/// ルートシグネチャ
/// </summary>
class RootSignature :public IRootSignature
{
private:

	/// <summary>
	/// パラメーター
	/// </summary>
	std::vector<D3D12_ROOT_PARAMETER>params;

	/// <summary>
	/// レンジ
	/// </summary>
	std::vector<D3D12_DESCRIPTOR_RANGE>ranges;

	std::vector<uint32_t>rangeLocation;

	/// <summary>
	/// サンプラー
	/// </summary>
	std::vector<D3D12_STATIC_SAMPLER_DESC>samplers;

	/// <summary>
	/// ルートシグネチャ
	/// </summary>
	Microsoft::WRL::ComPtr<ID3D12RootSignature>rootSignature;

public:
	
	/// <summary>
	/// ルートパラメータを追加
	/// </summary>
	void Add(RootType type_, uint32_t shaderRegister_, uint32_t registerSpace_ = 0);

	/// <summary>
	/// レンジを追加
	/// </summary>
	void Add(RangeType type_, uint32_t shaderRegister_, uint32_t registerSpace_ = 0, uint32_t descriptorCount_ = 1);

	/// <summary>
	/// サンプラーを追加
	/// </summary>
	void AddStaticSampler(
		uint32_t shaderRegister_,
		uint32_t registerSpace_ = 0,
		D3D12_FILTER filter_ = D3D12_FILTER_MIN_MAG_MIP_LINEAR,
		AddressMode addressU_ = AddressMode::Wrap,
		AddressMode addressV_ = AddressMode::Wrap,
		AddressMode addressW_ = AddressMode::Wrap
	);

	/// <summary>
	/// 生成
	/// </summary>
	void Clear();

	/// <summary>
	/// 生成
	/// </summary>
	void Create(ID3D12Device* device_);

	/// <summary>
	/// ルートシグネチャ取得
	/// </summary>
	ID3D12RootSignature* GetRootSignature();

	RootSignature() = default;
	~RootSignature() = default;
};

void RootSignature::Add(RootType type_, uint32_t shaderRegister_, uint32_t registerSpace_)
{
	//ルートパラメータの設定
	D3D12_ROOT_PARAMETER lRootParam{};
	//全てのシェーダから見える
	lRootParam.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
	//タイプ
	lRootParam.ParameterType = static_cast<D3D12_ROOT_PARAMETER_TYPE>(type_);
	//バッファ番号
	lRootParam.Descriptor.ShaderRegister = shaderRegister_;

	lRootParam.Descriptor.RegisterSpace = registerSpace_;

	params.push_back(lRootParam);
}

void RootSignature::Add(RangeType type_, uint32_t shaderRegister_, uint32_t registerSpace_, uint32_t descriptorCount_)
{
	D3D12_DESCRIPTOR_RANGE lRange{};
	//タイプ
	lRange.RangeType = static_cast<D3D12_DESCRIPTOR_RANGE_TYPE>(type_);
	lRange.NumDescriptors = descriptorCount_;
	lRange.BaseShaderRegister = shaderRegister_;
	lRange.RegisterSpace = registerSpace_;
	lRange.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
	ranges.push_back(lRange);

	uint32_t lRangeIndex = uint32_t(params.size());
	rangeLocation.push_back(lRangeIndex);

	D3D12_ROOT_PARAMETER lRootParam{};
	//全てのシェーダから見える
	lRootParam.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
	//タイプ
	lRootParam.DescriptorTable.NumDescriptorRanges = 1;
	//レンジ
	lRootParam.DescriptorTable.pDescriptorRanges = nullptr; // ここは生成時入れる

	params.push_back(lRootParam);
}

void RootSignature::AddStaticSampler(uint32_t shaderRegister_, uint32_t registerSpace_, D3D12_FILTER filter_, AddressMode addressU_, AddressMode addressV_, AddressMode addressW_)
{
	CD3DX12_STATIC_SAMPLER_DESC lDesc{};
	lDesc.Init(shaderRegister_,
		filter_,
		static_cast<D3D12_TEXTURE_ADDRESS_MODE>(addressU_),
		static_cast<D3D12_TEXTURE_ADDRESS_MODE>(addressV_),
		static_cast<D3D12_TEXTURE_ADDRESS_MODE>(addressW_)
	);
	lDesc.RegisterSpace = registerSpace_;
	samplers.push_back(lDesc);
}

void RootSignature::Clear()
{
	params.clear();
	ranges.clear();
	rangeLocation.clear();
	samplers.clear();
}

void RootSignature::Create(ID3D12Device* device)
{
	for (size_t i = 0; i < ranges.size(); ++i)
	{
		uint32_t lIndex = rangeLocation[i];

		//レンジのアドレス代入
		params[lIndex].DescriptorTable.pDescriptorRanges = &ranges[i];
	}

	D3D12_ROOT_SIGNATURE_DESC lDesc{};

	if (!params.empty())
	{
		lDesc.pParameters = params.data();
		lDesc.NumParameters = static_cast<UINT>(params.size());
	}

	if (!samplers.empty())
	{
		lDesc.pStaticSamplers = samplers.data();
		lDesc.NumStaticSamplers = static_cast<UINT>(samplers.size());
	}

	lDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

	Microsoft::WRL::ComPtr<ID3DBlob> lBlob, lErrBlob;

	HRESULT lResult = D3D12SerializeRootSignature(&lDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, &lBlob, &lErrBlob);

	lResult = device->CreateRootSignature(0, lBlob->GetBufferPointer(), lBlob->GetBufferSize(), IID_PPV_ARGS(rootSignature.GetAddressOf()));
	
	if (FAILED(lResult))
	{
		assert(0);
	}
}

 ID3D12RootSignature* RootSignature::GetRootSignature()
{
	return rootSignature.Get();
}

 std::unique_ptr<IRootSignature> CreateUniqueRootSignature()
 {
	 std::unique_ptr<IRootSignature> lRootSignature = std::make_unique<RootSignature>();
	 return std::move(lRootSignature);
 }

 std::shared_ptr<IRootSignature> CreateSharedRootSignature()
 {
	 std::shared_ptr<IRootSignature> lRootSignature = std::make_shared<RootSignature>();
	 return lRootSignature;
 }
