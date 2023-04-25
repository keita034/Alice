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
/// インデックスバッファ
/// </summary>
class IndexBuffer : public BaseBuffer
{
private:
	
	// インデックスバッファビュー
	D3D12_INDEX_BUFFER_VIEW bufferView{};

	void* bufferMappedPtr = nullptr;

	size_t bufferLength = 0;

public:

	/// <summary>
	/// インデックスバッファを生成
	/// </summary>
	/// <param name="length">インデックスバッファの要素数</param>
	/// <param name="data">インデックス配列の先頭アドレス(uint32_t)</param>
	void Create(size_t length, const uint32_t* data = nullptr);

	/// <summary>
	/// 成功したか
	/// </summary>
	bool IsValid();

	/// <summary>
	/// インデックスバッファビューを取得
	/// </summary>
	D3D12_INDEX_BUFFER_VIEW GetView() const;

	/// <summary>
	/// データの更新
	/// </summary>
	/// <param name="data">データ</param>
	void Update(void* data);

	IndexBuffer() = default;
	~IndexBuffer() = default;

private:

	IndexBuffer(const IndexBuffer&) = delete;
	void operator = (const IndexBuffer&) = delete;
};

