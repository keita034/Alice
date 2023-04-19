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
/// 頂点バッファ
/// </summary>
class VertexBuffer : public BaseBuffer
{
private:

	// 頂点バッファビュー
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView = {};

	void* bufferMappedPtr = nullptr;

public:
	
	/// <summary>
	/// バッファを生成
	/// </summary>
	/// <param name="length">要素数</param>
	/// <param name="singleSize">単体のサイズ</param>
	/// <param name="data">配列の先頭アドレス</param>
	void Create(size_t length, size_t singleSize, const void* data = nullptr);
	
	/// <summary>
	/// 頂点バッファビューを取得
	/// </summary>
	/// <returns>頂点バッファビュー</returns>
	D3D12_VERTEX_BUFFER_VIEW GetView() const;

	/// <summary>
	/// バッファを取得
	/// </summary>
	/// <returns>バッファ</returns>
	ID3D12Resource* GetResource();

	/// <summary>
	/// バッファの生成に成功したかを取得
	/// </summary>
	/// <returns>成功したか</returns>
	bool IsValid();

	/// <summary>
	/// データの更新
	/// </summary>
	/// <param name="data">データ</param>
	void Update(void* data);

	void CommonInitialize(ID3D12Device* dev);
	
	~VertexBuffer() = default;
	VertexBuffer() = default;

private:

	VertexBuffer(const VertexBuffer&) = delete;
	void operator = (const VertexBuffer&) = delete;
};

