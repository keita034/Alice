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
/// コンピュートシェーダー用頂点バッファ(インターフェース)
/// </summary>
class IComputeVertexBuffer
{
public:

	/// <summary>
	/// バッファを生成
	/// </summary>
	/// <param name="length_">要素数</param>
	/// <param name="singleSize">単体のサイズ</param>
	/// <param name="data">配列のポインタ</param>
	virtual void Create(size_t length_, size_t singleSize_, const void* data_ = nullptr) = 0;

	/// <summary>
	/// 頂点バッファビューを取得
	/// </summary>
	/// <returns>頂点バッファビュー</returns>
	virtual const D3D12_VERTEX_BUFFER_VIEW& GetView() = 0;

	/// <summary>
	/// リソースを取得
	/// </summary>
	virtual ID3D12Resource* GetResource() = 0;

	/// <summary>
	/// バッファの生成に成功したかを取得
	/// </summary>
	virtual bool IsValid() = 0;

	/// <summary>
	/// バッファのGPU上のアドレスを返す
	/// </summary>
	/// <returns>バッファのGPU上のアドレス</returns>
	virtual const D3D12_GPU_DESCRIPTOR_HANDLE& GetAddress() = 0;

	/// <summary>
	/// バッファの状態を変える
	/// </summary>
	/// <param name="beforeState">今の状態</param>
	/// <param name="afterState">変えたい状態</param>
	virtual void Transition(const D3D12_RESOURCE_STATES& beforeState_, const D3D12_RESOURCE_STATES& afterState_) = 0;

	/// <summary>
	/// データ更新
	/// </summary>
	/// <param name="data">データ</param>
	virtual void Update(void* data_) = 0;

	virtual ~IComputeVertexBuffer() = default;
	IComputeVertexBuffer() = default;

private:

	IComputeVertexBuffer(const IComputeVertexBuffer&) = delete;
	IComputeVertexBuffer& operator = (const IComputeVertexBuffer&) = delete;
};

/// <summary>
/// コンピュートシェーダーの生成(ユニーク)
/// </summary>
/// <param name="length_">要素数</param>
/// <param name="singleSize_">単体のサイズ</param>
/// <param name="data">配列の先頭アドレス</param>
/// <returns>生成されたポインタ</returns>
std::unique_ptr<IComputeVertexBuffer> CreateUniqueComputeVertexBuffer(size_t length_, size_t singleSize_, const void* data_ = nullptr);

/// <summary>
/// コンピュートシェーダーの生成(シェアード)
/// </summary>
/// <param name="length_">要素数</param>
/// <param name="singleSize_">単体のサイズ</param>
/// <param name="data">配列の先頭アドレス</param>
/// <returns>生成されたポインタ</returns>
std::shared_ptr<IComputeVertexBuffer> CreateSharedComputeVertexBuffer(size_t length_, size_t singleSize_, const void* data_ = nullptr);