#pragma once
#include<BaseBuffer.h>

class CrossAdapterBuffer : public BaseBuffer
{
private:

	Microsoft::WRL::ComPtr<ID3D12Heap> heap;
	//個数
	size_t bufferlength;
	//データ一つ分のサイズ
	size_t bufferSingleSize;
	//GPUハンドル
	D3D12_GPU_DESCRIPTOR_HANDLE structuredBufferHandle;

	D3D12_SHADER_RESOURCE_VIEW_DESC srvResDesc{};

	int8_t PADING[ 4 ];

public:

	/// <summary>
	/// 生成
	/// </summary>
	/// <param name="length_">個数</param>
	/// <param name="singleSize__">要素1つ分のデータサイズ</param>
	void Create(size_t length_,size_t singleSize_);

	/// <summary>
	/// バッファ生成に成功したかを返す
	/// </summary>
	bool IsValid();

	/// <summary>
	/// バッファのGPU上のアドレスを返す
	/// </summary>
	/// <returns>バッファのGPU上のアドレス</returns>
	const D3D12_GPU_DESCRIPTOR_HANDLE& GetAddress();

	/// <summary>
	/// バッファを取得
	/// </summary>
	ID3D12Resource* GetResource();

	~CrossAdapterBuffer() = default;
	CrossAdapterBuffer() = default;

private:

	CrossAdapterBuffer(const CrossAdapterBuffer&) = delete;
	CrossAdapterBuffer& operator = (const CrossAdapterBuffer&) = delete;
};

