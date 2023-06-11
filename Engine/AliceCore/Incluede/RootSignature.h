#pragma once
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

#include<directx/d3d12.h>
#include<memory>

#pragma warning(pop)

/// <summary>
/// ルートシグネチャ(インターフェース)
/// </summary>
class IRootSignature
{
public:

	/// <summary>
	/// ルートパラメータタイプ
	/// </summary>
	enum class RootType
	{
		CBV = D3D12_ROOT_PARAMETER_TYPE_CBV,
		SRV = D3D12_ROOT_PARAMETER_TYPE_SRV,
		UAV = D3D12_ROOT_PARAMETER_TYPE_UAV,
	};

	/// <summary>
	/// レンジタイプ
	/// </summary>
	enum class RangeType
	{
		SRV = D3D12_DESCRIPTOR_RANGE_TYPE_SRV,
		UAV = D3D12_DESCRIPTOR_RANGE_TYPE_UAV,
		CBV = D3D12_DESCRIPTOR_RANGE_TYPE_CBV,
		Sampler = D3D12_DESCRIPTOR_RANGE_TYPE_SAMPLER
	};
	enum class AddressMode
	{
		Wrap = D3D12_TEXTURE_ADDRESS_MODE_WRAP,
		Mirror = D3D12_TEXTURE_ADDRESS_MODE_MIRROR,
		Clamp = D3D12_TEXTURE_ADDRESS_MODE_CLAMP,
		Border = D3D12_TEXTURE_ADDRESS_MODE_BORDER,
		MirrorOnce = D3D12_TEXTURE_ADDRESS_MODE_MIRROR_ONCE
	};

	/// <summary>
	/// ルートパラメータを追加
	/// </summary>
	/// <param name="type">種類</param>
	/// <param name="shaderRegister">バッファ番号</param>
	/// <param name="registerSpace">レジスタースペース</param>
	virtual void Add(RootType type_, uint32_t shaderRegister_, uint32_t registerSpace_ = 0) = 0;

	/// <summary>
	/// レンジを追加
	/// </summary>
	/// <param name="type">種類</param>
	/// <param name="shaderRegister">バッファ番号</param>
	/// <param name="registerSpace">レジスタースペース</param>
	/// <param name="descriptorCount">一度に使うテクスチャや定数バッファ</param>
	virtual void Add(RangeType type_, uint32_t shaderRegister_, uint32_t registerSpace_ = 0, uint32_t descriptorCount_ = 1) = 0;

	/// <summary>
	/// サンプラーを追加
	/// </summary>
	/// <param name="shaderRegister">バッファ番号</param>
	/// <param name="registerSpace">レジスタースペース</param>
	/// <param name="filter">フィルター</param>
	/// <param name="addressU">Uのアドレスモード</param>
	/// <param name="addressV">Vのアドレスモード</param>
	/// <param name="addressW">Wのアドレスモード</param>
	virtual void AddStaticSampler(
		uint32_t shaderRegister_,
		uint32_t registerSpace_ = 0,
		D3D12_FILTER filter_ = D3D12_FILTER_MIN_MAG_MIP_LINEAR,
		AddressMode addressU_ = AddressMode::Wrap,
		AddressMode addressV_ = AddressMode::Wrap,
		AddressMode addressW_ = AddressMode::Wrap
	) = 0;

	/// <summary>
	/// 生成
	/// </summary>
	virtual void Clear() = 0;

	/// <summary>
	/// 生成
	/// </summary>
	/// <param name="device">デバイス</param>
	virtual void Create(ID3D12Device* device_) = 0;

	/// <summary>
	/// ルートシグネチャ取得
	/// </summary>
	virtual ID3D12RootSignature* GetRootSignature() = 0;

	IRootSignature() = default;
	virtual ~IRootSignature() = default;
};

/// <summary>
/// ルートシグネチャの生成(ユニーク)
/// </summary>
/// <returns>生成されたポインタ</returns>
std::unique_ptr<IRootSignature> CreateUniqueRootSignature();

/// <summary>
/// ルートシグネチャの生成(シェアード)
/// </summary>
/// <returns>生成されたポインタ</returns>
std::shared_ptr<IRootSignature> CreateSharedRootSignature();