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

#include<AdaptersIndex.h>

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
	virtual void Create(size_t length_,AdaptersIndex index_,const uint32_t* data_) = 0;

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

	/// <summary>
	/// データの更新
	/// </summary>
	/// <param name="data">データ</param>
	/// <param name="length">データの長さ</param>
	virtual void Update(void* data_,size_t length_) = 0;

	/// <summary>
	/// 名前を設定
	/// </summary>
	/// <param name="name_">名前</param>
	virtual void SetName(const std::string& name_) = 0;

	virtual void CreateSRV() = 0;

	virtual const D3D12_GPU_DESCRIPTOR_HANDLE& GetSRVAddress() = 0;

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
std::unique_ptr<IIndexBuffer> CreateUniqueIndexBuffer(size_t length_,AdaptersIndex index_ = AdaptersIndex::MAIN,const uint32_t* data_ = nullptr);

/// <summary>
/// インデックスバッファの生成(シェアード)
/// </summary>
/// <param name="length_">インデックスバッファの要素数</param>
/// <param name="data">インデックス配列の先頭アドレス(uint32_t)</param>
/// <returns>生成されたポインタ</returns>
std::shared_ptr<IIndexBuffer> CreateSharedIndexBuffer(size_t length_,AdaptersIndex index_ = AdaptersIndex::MAIN,const uint32_t* data_ = nullptr);