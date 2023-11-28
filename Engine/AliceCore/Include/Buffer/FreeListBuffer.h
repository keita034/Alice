#pragma once

#pragma warning(push)
#pragma warning(disable: 4061)
#pragma warning(disable: 4062)
#pragma warning(disable: 4514)
#pragma warning(disable: 4365)
#pragma warning(disable: 4668)
#pragma warning(disable: 4820)
#pragma warning(disable: 5039)

#include<directx/d3dx12.h>
#include<memory>

#pragma warning(pop)


#include<CrossAdapterResourceIndex.h>
#include<AdaptersIndex.h>
#include<BufferType.h>

class IFreeListBuffer
{

public:

	/// <summary>
	/// 定数バッファを生成
	/// </summary>
	/// <param name="size">データサイズ</param>
	virtual void Create(size_t length_,size_t singleSize_,BufferType type_,AdaptersIndex mainIndex_,AdaptersIndex subIndex_) = 0;

	/// <summary>
	/// バッファ生成に成功したかを返す
	/// </summary>
	/// <returns>バッファ生成に成功したか</returns>
	virtual bool IsValid() = 0;

	/// <summary>
	/// バッファのGPU上のアドレスを返す
	/// </summary>
	/// <returns>バッファのGPU上のアドレス</returns>
	virtual const D3D12_GPU_DESCRIPTOR_HANDLE& GetAddress() = 0;

	/// <summary>
	/// バッファのGPU上のアドレスを返す
	/// </summary>
	/// <returns>バッファのGPU上のアドレス</returns>
	virtual const D3D12_GPU_DESCRIPTOR_HANDLE& GetAddress(CrossAdapterResourceIndex index_) = 0;

	/// <summary>
	/// バッファを取得
	/// </summary>
	virtual ID3D12Resource* GetResource() = 0;

	/// <summary>
	/// バッファを取得
	/// </summary>
	virtual ID3D12Resource* GetResource(CrossAdapterResourceIndex index_) = 0;

	IFreeListBuffer() = default;
	virtual ~IFreeListBuffer() = default;

private:

	virtual uint32_t PAlignForUavCounter(uint32_t bufferSize_) = 0;
};

std::unique_ptr<IFreeListBuffer> CreateUniqueFreeListBuffer(size_t length_,size_t singleSize_,BufferType type_ = BufferType::MAIN,AdaptersIndex mainIndex_ = AdaptersIndex::MAIN,AdaptersIndex subIndex_ = AdaptersIndex::SUB);

std::shared_ptr<IFreeListBuffer> CreateSharedFreeListBuffer(size_t length_,size_t singleSize_,BufferType type_ = BufferType::MAIN,AdaptersIndex mainIndex_ = AdaptersIndex::MAIN,AdaptersIndex subIndex_ = AdaptersIndex::SUB);
