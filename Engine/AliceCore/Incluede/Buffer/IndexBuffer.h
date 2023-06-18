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

/// <summary>
/// インデックスバッファ(インタフェース)
/// </summary>
class IIndexBuffer
{
public:

	/// <summary>
	/// インデックスバッファを生成
	/// </summary>
	/// <param name="length_">インデックスバッファの要素数</param>
	/// <param name="data">インデックス配列の先頭アドレス(uint32_t)</param>
	virtual void Create(size_t length_, const uint32_t* data_) = 0;

	/// <summary>
	/// 成功したか
	/// </summary>
	virtual bool IsValid() = 0;

	/// <summary>
	/// インデックスバッファビューを取得
	/// </summary>
	virtual const D3D12_INDEX_BUFFER_VIEW& GetView() = 0;

	/// <summary>
	/// データの更新
	/// </summary>
	/// <param name="data">データ</param>
	virtual void Update(void* data_) = 0;

	IIndexBuffer() = default;
	virtual ~IIndexBuffer() = default;

private:

	IIndexBuffer(const IIndexBuffer&) = delete;
	IIndexBuffer& operator = (const IIndexBuffer&) = delete;
};

/// <summary>
/// インデックスバッファの生成(ユニーク)
/// </summary>
/// <param name="length_">インデックスバッファの要素数</param>
/// <param name="data">インデックス配列の先頭アドレス(uint32_t)</param>
/// <returns>生成されたポインタ</returns>
std::unique_ptr<IIndexBuffer> CreateUniqueIndexBuffer(size_t length_, const uint32_t* data_ = nullptr);

/// <summary>
/// インデックスバッファの生成(シェアード)
/// </summary>
/// <param name="length_">インデックスバッファの要素数</param>
/// <param name="data">インデックス配列の先頭アドレス(uint32_t)</param>
/// <returns>生成されたポインタ</returns>
std::shared_ptr<IIndexBuffer> CreateSharedIndexBuffer(size_t length_, const uint32_t* data_ = nullptr);