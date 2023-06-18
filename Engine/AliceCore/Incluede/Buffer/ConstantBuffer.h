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
/// 定数バッファ(インターフェース)
/// </summary>
class IConstantBuffer
{
public:
	/// <summary>
	/// 定数バッファを生成
	/// </summary>
	/// <param name="size">データサイズ</param>
	virtual void Create(size_t bufferSize_) = 0;

	/// <summary>
	/// バッファ生成に成功したかを返す
	/// </summary>
	/// <returns>バッファ生成に成功したか</returns>
	virtual bool IsValid() = 0;

	/// <summary>
	/// バッファのGPU上のアドレスを返す
	/// </summary>
	/// <returns>バッファのGPU上のアドレス</returns>
	virtual const D3D12_GPU_VIRTUAL_ADDRESS& GetAddress() = 0;

	/// <summary>
	/// 定数バッファビューを返す
	/// </summary>
	/// <returns>定数バッファビュー</returns>
	virtual const D3D12_CONSTANT_BUFFER_VIEW_DESC& GetViewDesc() = 0;

	/// <summary>
	/// データの更新
	/// </summary>
	/// <param name="data">データ</param>
	virtual void Update(void* data_) = 0;

	/// <summary>
	/// バッファを取得
	/// </summary>
	virtual ID3D12Resource* GetResource() = 0;

	/// <summary>
	/// マップ用ポインタを取得
	/// </summary>
	virtual void* GetPtr() = 0;

	virtual ~IConstantBuffer() = default;
	IConstantBuffer() = default;


private:

	IConstantBuffer(const IConstantBuffer&) = delete;
	IConstantBuffer& operator = (const IConstantBuffer&) = delete;

};

/// <summary>
/// 定数バッファの生成(ユニーク)
/// </summary>
/// <param name="size">データサイズ</param>
/// <returns>生成されたポインタ</returns>
std::unique_ptr<IConstantBuffer> CreateUniqueConstantBuffer(size_t bufferSize_);

/// <summary>
/// 定数バッファの生成(シェアード)
/// </summary>
/// <param name="size">データサイズ</param>
/// <returns>生成されたポインタ</returns>
std::shared_ptr<IConstantBuffer> CreateSharedConstantBuffer(size_t bufferSize_);