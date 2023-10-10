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

struct CD3DX12_RESOURCE_DESC;
struct D3D12_GPU_DESCRIPTOR_HANDLE;
struct D3D12_SHADER_RESOURCE_VIEW_DESC;
struct ID3D12Resource;

#include<MultiAdapters.h>

enum HEAP_TYPE
{
	DEFAULT = 1,
	UPLOAD = 2,
	READBACK = 3,
	CUSTOM = 4,
	GPU_UPLOAD = 5
};

/// <summary>
/// 書き込みと読み込み用構造化バッファ(インタフェース)
/// </summary>
class IRWStructuredBuffer
{
public:

	/// <summary>
	/// シェーダーリソースビューを生成
	/// </summary>
	/// <param name="length_">要素数</param>
	/// <param name="singleSize_">要素1つ分のデータサイズ</param>
	/// <param name="data_">データ</param>
	virtual void CreateSRV(size_t length_, size_t singleSize_,AdaptersIndex index_, const void* data_) = 0;

	/// <summary>
	/// アンオーダーアクセスビューを生成
	/// </summary>
	/// <param name="length_">個数</param>
	/// <param name="singleSize__">要素1つ分のデータサイズ</param>
	/// <param name="data_">データ</param>
	virtual void CreateUAV(size_t length_, size_t singleSize_,AdaptersIndex index_, const void* data_) = 0;

	/// <summary>
	/// アンオーダーアクセスビューを生成
	/// </summary>
	virtual void CreateUAV(CD3DX12_RESOURCE_DESC* texresDesc_,AdaptersIndex index_) = 0;

	/// <summary>
	/// バッファ生成に成功したかを返す
	/// </summary>
	virtual bool IsValid() = 0;

	/// <summary>
	/// バッファのGPU上のアドレスを返す
	/// </summary>
	/// <returns>バッファのGPU上のアドレス</returns>
	virtual const D3D12_GPU_DESCRIPTOR_HANDLE& GetAddress() = 0;

	/// <summary>
	/// 定数バッファビューを返す
	/// </summary>
	/// <returns>定数バッファビュー</returns>
	virtual const D3D12_SHADER_RESOURCE_VIEW_DESC& GetViewDesc() = 0;

	/// <summary>
	/// バッファを取得
	/// </summary>
	virtual ID3D12Resource* GetResource() = 0;

	/// <summary>
	/// 定数バッファにマッピングされたポインタを返す
	/// </summary>
	/// <returns>定数バッファにマッピングされたポインタ</returns>
	virtual void* GetPtr() const = 0;

	/// <summary>
	/// データを更新
	/// </summary>
	virtual void Update(void* data_) = 0;

	/// <summary>
	/// ヒープタイプを設定
	/// </summary>
	/// <param name="flag_"></param>
	virtual void SetHeapType(HEAP_TYPE heapType_) = 0;

	virtual ~IRWStructuredBuffer() = default;
	IRWStructuredBuffer() = default;

private:

	IRWStructuredBuffer(const IRWStructuredBuffer&) = delete;
	IRWStructuredBuffer& operator = (const IRWStructuredBuffer&) = delete;

};

/// <summary>
/// シェーダーリソースビューを生成(ユニーク)
/// </summary>
/// <param name="length_">要素数</param>
/// <param name="singleSize_">要素1つ分のデータサイズ</param>
/// <param name="heapType_">ヒープタイプ</param>
/// <param name="data_">データ</param>
/// <returns>生成されたポインタ</returns>
std::unique_ptr<IRWStructuredBuffer> CreateUniqueSRVRWStructuredBuffer(size_t length_, size_t singleSize_,AdaptersIndex index_ = AdaptersIndex::MAIN,HEAP_TYPE heapType_ = UPLOAD, const void* data_ = nullptr);

/// <summary>
/// シェーダーリソースビューを生成(シェアード)
/// </summary>
/// <param name="length_">要素数</param>
/// <param name="singleSize_">要素1つ分のデータサイズ</param>
/// <param name="heapType_">ヒープタイプ</param>
/// <param name="data_">データ</param>
/// <returns>生成されたポインタ</returns>
std::shared_ptr<IRWStructuredBuffer> CreateSharedSRVRWStructuredBuffer(size_t length_, size_t singleSize_,AdaptersIndex index_ = AdaptersIndex::MAIN,HEAP_TYPE heapType_ = UPLOAD, const void* data_ = nullptr);

/// <summary>
/// アンオーダーアクセスビューを生成(ユニーク)
/// </summary>
/// <param name="length_">個数</param>
/// <param name="singleSize__">要素1つ分のデータサイズ</param>
/// <param name="heapType_">ヒープタイプ</param>
/// <param name="data_">データ</param>
/// <returns>生成されたポインタ</returns>
std::unique_ptr<IRWStructuredBuffer> CreateUniqueUAVRWStructuredBuffer(size_t length_, size_t singleSize_,AdaptersIndex index_ = AdaptersIndex::MAIN,HEAP_TYPE heapType_ = UPLOAD, const void* data_ = nullptr);

/// <summary>
/// アンオーダーアクセスビューを生成(シェアード)
/// </summary>
/// <param name="length_">個数</param>
/// <param name="singleSize__">要素1つ分のデータサイズ</param>
/// <param name="heapType_">ヒープタイプ</param>
/// <param name="data_">データ</param>
/// <returns>生成されたポインタ</returns>
std::shared_ptr<IRWStructuredBuffer> CreateSharedUAVRWStructuredBuffer(size_t length_, size_t singleSize_,AdaptersIndex index_ = AdaptersIndex::MAIN,HEAP_TYPE heapType_ = UPLOAD, const void* data_ = nullptr);