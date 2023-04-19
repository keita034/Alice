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
/// 定数バッファ
/// </summary>
class ConstantBuffer : public BaseBuffer
{

private:

	// 定数バッファビューの設定
	D3D12_CONSTANT_BUFFER_VIEW_DESC constantBufferView = {};

	void* bufferMappedPtr = nullptr;

	size_t bufferSize;

public:
	/// <summary>
	/// 定数バッファを生成
	/// </summary>
	/// <param name="size">データサイズ</param>
	void Create(size_t size);

	/// <summary>
	/// バッファ生成に成功したかを返す
	/// </summary>
	/// <returns>バッファ生成に成功したか</returns>
	bool IsValid();

	/// <summary>
	/// バッファのGPU上のアドレスを返す
	/// </summary>
	/// <returns>バッファのGPU上のアドレス</returns>
	D3D12_GPU_VIRTUAL_ADDRESS GetAddress() const;

	/// <summary>
	/// 定数バッファビューを返す
	/// </summary>
	/// <returns>定数バッファビュー</returns>
	D3D12_CONSTANT_BUFFER_VIEW_DESC GetViewDesc();

	/// <summary>
	/// データの更新
	/// </summary>
	/// <param name="data">データ</param>
	void Update(void* data);

	/// <summary>
	/// バッファを取得
	/// </summary>
	ID3D12Resource* GetResource();

	/// <summary>
	/// マップ用ポインタを取得
	/// </summary>
	/// <returns></returns>
	void* GetPtr();

	~ConstantBuffer() = default;
	ConstantBuffer() = default;


private:

	ConstantBuffer(const ConstantBuffer&) = delete;
	void operator = (const ConstantBuffer&) = delete;

};