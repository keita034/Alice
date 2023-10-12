#pragma once
#include<AdaptersIndex.h>
#include<Defined.h>

ALICE_SUPPRESS_WARNINGS_BEGIN

#include<memory>

ALICE_SUPPRESS_WARNINGS_END

struct D3D12_GPU_DESCRIPTOR_HANDLE;
struct ID3D12Resource;

class ICrossAdapterBuffer
{
public:

	enum ResourceIndex
	{
		SAUCE,//コピー元
		DUST,///コピー先
	};

public:

	/// <summary>
	/// 生成
	/// </summary>
	/// <param name="length_">個数</param>
	/// <param name="singleSize__">要素1つ分のデータサイズ</param>
	virtual void Create(size_t length_,size_t singleSize_,AdaptersIndex dustIndex_,AdaptersIndex sauceIndex_) = 0;

	/// <summary>
	/// バッファ生成に成功したかを返す
	/// </summary>
	virtual bool IsValid() = 0;

	/// <summary>
	/// バッファのGPU上のアドレスを返す
	/// </summary>
	/// <returns>バッファのGPU上のアドレス</returns>
	virtual const D3D12_GPU_DESCRIPTOR_HANDLE& GetAddress(ResourceIndex index_) = 0;

	/// <summary>
	/// バッファを取得
	/// </summary>
	virtual ID3D12Resource* GetResource() = 0;

	virtual void ResourceCopy() =0;

	virtual ~ICrossAdapterBuffer() = default;
	ICrossAdapterBuffer() = default;

private:

	ICrossAdapterBuffer(const ICrossAdapterBuffer&) = delete;
	ICrossAdapterBuffer& operator = (const ICrossAdapterBuffer&) = delete;
};

std::unique_ptr<ICrossAdapterBuffer> CreateUniqueCrossAdapterBuffer(size_t length_,size_t singleSize_,AdaptersIndex dustIndex_,AdaptersIndex sauceIndex_);

std::shared_ptr<ICrossAdapterBuffer> CreateSharedCrossAdapterBuffer(size_t length_,size_t singleSize_,AdaptersIndex dustIndex_,AdaptersIndex sauceIndex_);