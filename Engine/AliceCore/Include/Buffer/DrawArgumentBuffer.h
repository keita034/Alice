#pragma once
#include<BaseBuffer.h>

#include<Defined.h>

ALICE_SUPPRESS_WARNINGS_BEGIN

#include<directx/d3dx12.h>

ALICE_SUPPRESS_WARNINGS_END

/// <summary>
/// ドローアーギュメントバッファ
/// </summary>
class IDrawArgumentBuffer
{
public:

	/// <summary>
	/// バッファを生成
	/// </summary>
	/// <param name="length_">要素数</param>
	/// <param name="singleSize_">単体のサイズ</param>
	/// <param name="data_">配列の先頭アドレス</param>
	virtual void Create(size_t length_,size_t singleSize_,const void* data_) = 0;

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
	/// バッファを取得
	/// </summary>
	virtual ID3D12Resource* GetResource() = 0;

	/// <summary>
	/// バッファにマッピングされたポインタを返す
	/// </summary>
	/// <returns>バッファにマッピングされたポインタ</returns>
	virtual void* GetPtr() const = 0;

	/// <summary>
	/// データの更新
	/// </summary>
	/// <param name="data_">データ</param>
	virtual void Update(void* data_) = 0;

	/// <summary>
	/// ステータス変更
	/// </summary>
	/// <param name="statesAfter_">変更したいステータス</param>
	virtual void Transition(D3D12_RESOURCE_STATES statesAfter_) = 0;

	/// <summary>
	/// ステータスを取得
	/// </summary>
	/// <returns></returns>
	virtual D3D12_RESOURCE_STATES GetStates() = 0;

	/// <summary>
	/// ヒープタイプを設定
	/// </summary>
	/// <param name="heapType_">ヒープタイプ</param>
	virtual void SetHeapType(D3D12_HEAP_TYPE heapType_) = 0;

	virtual ~IDrawArgumentBuffer() = default;
	IDrawArgumentBuffer() = default;

private:

	IDrawArgumentBuffer(const IDrawArgumentBuffer&) = delete;
	IDrawArgumentBuffer& operator = (const IDrawArgumentBuffer&) = delete;
};


/// <summary>
/// ドローアーギュメントバッファを生成(ユニーク)
/// </summary>
/// <param name="length_">個数</param>
/// <param name="singleSize__">要素1つ分のデータサイズ</param>
/// <param name="heapType_">ヒープタイプ</param>
/// <param name="data_">データ</param>
/// <returns>生成されたポインタ</returns>
std::unique_ptr<IDrawArgumentBuffer> CreateUniqueDrawArgumentBuffer(size_t length_,size_t singleSize_,D3D12_HEAP_TYPE heapType_ = D3D12_HEAP_TYPE::D3D12_HEAP_TYPE_UPLOAD,const void* data_ = nullptr);

/// <summary>
/// ドローアーギュメントバッファを生成(シェアード)
/// </summary>
/// <param name="length_">個数</param>
/// <param name="singleSize__">要素1つ分のデータサイズ</param>
/// <param name="heapType_">ヒープタイプ</param>
/// <param name="data_">データ</param>
/// <returns>生成されたポインタ</returns>
std::shared_ptr<IDrawArgumentBuffer> CreateSharedDrawArgumentBuffer(size_t length_,size_t singleSize_,D3D12_HEAP_TYPE heapType_ = D3D12_HEAP_TYPE::D3D12_HEAP_TYPE_UPLOAD,const void* data_ = nullptr);
