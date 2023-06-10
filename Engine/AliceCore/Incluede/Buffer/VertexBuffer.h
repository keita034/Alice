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

#pragma warning(pop)

/// <summary>
/// 頂点バッファ(インターフェース)
/// </summary>
class IVertexBuffer
{
public:

	/// <summary>
	/// <para>バッファを生成</para>
	/// <para>頂点バッファの生成時に呼び出し済み</para>
	/// </summary>
	/// <param name="length_">要素数</param>
	/// <param name="singleSize">単体のサイズ</param>
	/// <param name="data">配列の先頭アドレス</param>
	virtual void Create(size_t length_, size_t singleSize_, const void* data_) = 0;

	/// <summary>
	/// 頂点バッファビューを取得
	/// </summary>
	/// <returns>頂点バッファビュー</returns>
	virtual const D3D12_VERTEX_BUFFER_VIEW& GetView() = 0;

	/// <summary>
	/// バッファを取得
	/// </summary>
	/// <returns>バッファ</returns>
	virtual ID3D12Resource* GetResource() = 0;

	/// <summary>
	/// バッファの生成に成功したかを取得
	/// </summary>
	/// <returns>成功したか</returns>
	virtual bool IsValid() = 0;

	/// <summary>
	/// データの更新
	/// </summary>
	/// <param name="data">データ</param>
	virtual void Update(void* data_) = 0;

	virtual ~IVertexBuffer() = default;
	IVertexBuffer() = default;
};

/// <summary>
/// 頂点バッファを生成(ユニーク)
/// </summary>
/// <param name="length_">要素数</param>
/// <param name="singleSize">単体のサイズ</param>
/// <param name="data">配列の先頭アドレス</param>
/// <returns>生成されたポインタ</returns>
std::unique_ptr<IVertexBuffer> CreateUniqueVertexBuffer(size_t length_, size_t singleSize_, const void* data_ = nullptr);

/// <summary>
/// 頂点バッファを生成(シェアード)
/// </summary>
/// <param name="length_">要素数</param>
/// <param name="singleSize">単体のサイズ</param>
/// <param name="data">配列の先頭アドレス</param>
/// <returns>生成されたポインタ</returns>
std::shared_ptr<IVertexBuffer> CreateSharedVertexBuffer(size_t length_, size_t singleSize_, const void* data_ = nullptr);
