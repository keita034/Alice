#pragma once

#pragma warning(push)
#pragma warning(disable: 4061)
#pragma warning(disable: 4514)
#pragma warning(disable: 4365)
#pragma warning(disable: 4668)
#pragma warning(disable: 4820)
#pragma warning(disable: 5039)

#include<directx/d3dx12.h>

#pragma warning(pop)

#include"BaseBuffer.h"

/// <summary>
/// 読み取り専用構造化バッファ
/// </summary>
class StructuredBuffer : public BaseBuffer
{

private:

	size_t bufferLength;
	size_t bufferSingleSize;

	void* BufferMappedPtr = nullptr;

	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};

	D3D12_GPU_DESCRIPTOR_HANDLE structuredBufferHandle;
public:

	/// <summary>
	/// バッファを生成
	/// </summary>
	/// <param name="length">要素数</param>
	/// <param name="singleSize">単体のサイズ</param>
	/// <param name="data">配列の先頭アドレス</param>
	void Create(size_t length, size_t singleSize, const void* data = nullptr);

	/// <summary>
	/// バッファ生成に成功したかを返す
	/// </summary>
	/// <returns>バッファ生成に成功したか</returns>
	bool IsValid();

	/// <summary>
	/// バッファのGPU上のアドレスを返す
	/// </summary>
	/// <returns>バッファのGPU上のアドレス</returns>
	D3D12_GPU_DESCRIPTOR_HANDLE GetAddress() const;

	/// <summary>
	/// バッファビューを返す
	/// </summary>
	/// <returns>バッファビュー</returns>
	D3D12_SHADER_RESOURCE_VIEW_DESC GetViewDesc();

	/// <summary>
	/// バッファにマッピングされたポインタを返す
	/// </summary>
	/// <returns>バッファにマッピングされたポインタ</returns>
	void* GetPtr() const;

	/// <summary>
	/// データの更新
	/// </summary>
	/// <param name="data">データ</param>
	void Update(void* data);

	~StructuredBuffer() = default;
	StructuredBuffer() = default;

private:

	StructuredBuffer(const StructuredBuffer&) = delete;
	void operator = (const StructuredBuffer&) = delete;

};

