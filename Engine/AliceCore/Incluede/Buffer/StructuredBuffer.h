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
/// 読み取り専用構造化バッファ (インタフェース)
/// </summary>
class IStructuredBuffer
{
public:

	/// <summary>
	/// バッファを生成
	/// </summary>
	/// <param name="length_">要素数</param>
	/// <param name="singleSize_">単体のサイズ</param>
	/// <param name="data_">配列の先頭アドレス</param>
	virtual void Create(size_t length_, size_t singleSize_, const void* data_) = 0;

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
	/// バッファビューを返す
	/// </summary>
	/// <returns>バッファビュー</returns>
	virtual const D3D12_SHADER_RESOURCE_VIEW_DESC& GetViewDesc() = 0;

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

	virtual ~IStructuredBuffer() = default;
	IStructuredBuffer() = default;

private:

	IStructuredBuffer(const IStructuredBuffer&) = delete;
	IStructuredBuffer& operator = (const IStructuredBuffer&) = delete;

};

/// <summary>
///  読み取り専用構造化バッファを生成(ユニーク)
/// </summary>
/// <param name="length_">要素数</param>
/// <param name="singleSize_">単体のサイズ</param>
/// <param name="data_">配列の先頭アドレス</param>
/// <returns>生成されたポインタ</returns>
std::unique_ptr<IStructuredBuffer> CreateUniqueStructuredBuffer(size_t length_, size_t singleSize_, const void* data_ = nullptr);

/// <summary>
/// 読み取り専用構造化バッファを生成(シェアード)
/// </summary>
/// <param name="length_">要素数</param>
/// <param name="singleSize_">単体のサイズ</param>
/// <param name="data_">配列の先頭アドレス</param>
/// <returns>生成されたポインタ</returns>
std::shared_ptr<IStructuredBuffer> CreateSharedStructuredBuffer(size_t length_, size_t singleSize_, const void* data_ = nullptr);
