#pragma once
#include<AdaptersIndex.h>
#include<Defined.h>

ALICE_SUPPRESS_WARNINGS_BEGIN

#include<memory>

ALICE_SUPPRESS_WARNINGS_END

#include<CrossAdapterResourceIndex.h>

struct D3D12_GPU_DESCRIPTOR_HANDLE;
struct ID3D12Resource;

/// <summary>
/// アダプター間で共有できるバッファ(UAV固定)
/// </summary>
class ICrossAdapterBuffer
{
public:

	/// <summary>
	/// 生成
	/// </summary>
	/// <param name="length_">個数</param>
	/// <param name="singleSize_">要素1つ分のデータサイズ</param>
	/// <param name="mainIndex_">ヒープが生成されるアダプター</param>
	/// <param name="subIndex_">ヒープが共有されるアダプター</param>
	virtual void Create(size_t length_,size_t singleSize_,AdaptersIndex mainIndex_,AdaptersIndex subIndex_) = 0;

	/// <summary>
	/// バッファ生成に成功したかを返す
	/// </summary>
	virtual bool IsValid() = 0;

	/// <summary>
	/// バッファのGPU上のアドレスを返す
	/// </summary>
	/// <returns>バッファのGPU上のアドレス</returns>
	virtual const D3D12_GPU_DESCRIPTOR_HANDLE& GetAddress(CrossAdapterResourceIndex index_) = 0;

	/// <summary>
	/// バッファを取得
	/// </summary>
	virtual ID3D12Resource* GetResource() = 0;

	virtual ~ICrossAdapterBuffer() = default;
	ICrossAdapterBuffer() = default;

private:

	ICrossAdapterBuffer(const ICrossAdapterBuffer&) = delete;
	ICrossAdapterBuffer& operator = (const ICrossAdapterBuffer&) = delete;
};

/// <summary>
/// アダプター間で共有できるバッファの生成(ユニーク)
/// </summary>
/// <param name="length_">個数</param>
/// <param name="singleSize_">要素1つ分のデータサイズ</param>
/// <param name="mainIndex_">ヒープが生成されるアダプター</param>
/// <param name="subIndex_">ヒープが共有されるアダプター</param>
/// <returns>生成されたポインタ</returns>
std::unique_ptr<ICrossAdapterBuffer> CreateUniqueCrossAdapterBuffer(size_t length_,size_t singleSize_,AdaptersIndex mainIndex_,AdaptersIndex subIndex_);

/// <summary>
/// アダプター間で共有できるバッファの生成(シェアード)
/// </summary>
/// <param name="length_">個数</param>
/// <param name="singleSize_">要素1つ分のデータサイズ</param>
/// <param name="mainIndex_">ヒープが生成されるアダプター</param>
/// <param name="subIndex_">ヒープが共有されるアダプター</param>
/// <returns>生成されたポインタ</returns>
std::shared_ptr<ICrossAdapterBuffer> CreateSharedCrossAdapterBuffer(size_t length_,size_t singleSize_,AdaptersIndex mainIndex_,AdaptersIndex subIndex_);