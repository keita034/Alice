#pragma once
#include<BaseBuffer.h>

#include<Defined.h>

ALICE_SUPPRESS_WARNINGS_BEGIN

#include<directx/d3dx12.h>

ALICE_SUPPRESS_WARNINGS_END

#include<CrossAdapterResourceIndex.h>
#include<BufferType.h>

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
	virtual void Create(size_t length_,size_t singleSize_,BufferType type_,AdaptersIndex mainIndex_,AdaptersIndex subIndex_) = 0;

	/// <summary>
	/// バッファ生成に成功したかを返す
	/// </summary>
	/// <returns>バッファ生成に成功したか</returns>
	virtual bool IsValid() = 0;

	/// <summary>
	/// バッファのGPU上のアドレスを返す
	/// </summary>
	/// <returns>バッファのGPU上のアドレス</returns>
	virtual const D3D12_GPU_DESCRIPTOR_HANDLE& GetAddress(CrossAdapterResourceIndex index_) = 0;

	/// <summary>
	/// バッファを取得
	/// </summary>
	virtual ID3D12Resource* GetResource(CrossAdapterResourceIndex index_) = 0;

	/// <summary>
	/// ステータス変更
	/// </summary>
	/// <param name="statesAfter_">変更したいステータス</param>
	virtual void Transition(D3D12_RESOURCE_STATES statesAfter_,CrossAdapterResourceIndex resourceIndex_,AdaptersIndex adaptersIndex_) = 0;

	/// <summary>
	/// ステータスを取得
	/// </summary>
	/// <returns></returns>
	virtual D3D12_RESOURCE_STATES GetStates(CrossAdapterResourceIndex index_) = 0;


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
	/// ステータス変更
	/// </summary>
	/// <param name="statesAfter_">変更したいステータス</param>
	virtual void Transition(D3D12_RESOURCE_STATES statesAfter_) = 0;

	/// <summary>
	/// ステータスを取得
	/// </summary>
	/// <returns></returns>
	virtual D3D12_RESOURCE_STATES GetStates() = 0;

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
std::unique_ptr<IDrawArgumentBuffer> CreateUniqueDrawArgumentBuffer(size_t length_,size_t singleSize_,BufferType type_ = BufferType::MAIN,AdaptersIndex mainIndex_ = AdaptersIndex::MAIN,AdaptersIndex subIndex_ = AdaptersIndex::SUB);

/// <summary>
/// ドローアーギュメントバッファを生成(シェアード)
/// </summary>
/// <param name="length_">個数</param>
/// <param name="singleSize__">要素1つ分のデータサイズ</param>
/// <param name="heapType_">ヒープタイプ</param>
/// <param name="data_">データ</param>
/// <returns>生成されたポインタ</returns>
std::shared_ptr<IDrawArgumentBuffer> CreateSharedDrawArgumentBuffer(size_t length_,size_t singleSize_,BufferType type_ = BufferType::MAIN,AdaptersIndex mainIndex_ = AdaptersIndex::MAIN,AdaptersIndex subIndex_ = AdaptersIndex::SUB);
