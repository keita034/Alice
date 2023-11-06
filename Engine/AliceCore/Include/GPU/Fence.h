#pragma once

#pragma warning(push)
#pragma warning(disable: 4061)
#pragma warning(disable: 4062)
#pragma warning(disable: 4365)
#pragma warning(disable: 4514)
#pragma warning(disable: 4365)
#pragma warning(disable: 4668)
#pragma warning(disable: 4820)
#pragma warning(disable: 5039)

#include<memory>

#pragma warning(pop)

struct ID3D12Device;
struct ID3D12CommandQueue;
struct ID3D12Fence;

/// <summary>
/// フェンス
/// </summary>
class IFence
{
public:

	IFence() = default;
	virtual ~IFence() = default;

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="device_">デバイス</param>
	virtual void Initialize(ID3D12Device* device_) = 0;

	/// <summary>
	/// コマンドの処理が完了するまで待つ
	/// </summary>
	virtual void Wait() = 0;

	virtual void CompulsionWait() = 0;

	/// <summary>
	/// コマンドの処理が完了するまで待つ
	/// </summary>
	virtual void Wait(ID3D12CommandQueue* queue_,IFence* fence,uint64_t Value) = 0;

	/// <summary>
	/// シグナルを送る
	/// </summary>
	/// <param name="queue_">キュー</param>
	virtual void Signal(ID3D12CommandQueue* queue_) = 0;

	/// <summary>
	/// フェンスを取得
	/// </summary>
	virtual ID3D12Fence* Get() = 0;

	virtual uint64_t GetFenceVal() =0;

	virtual uint64_t GetFenceValANDIncrement() = 0;

	virtual void Signal(ID3D12CommandQueue* queue_,uint64_t value_) = 0;

};

/// <summary>
/// フェンスの生成(ユニーク)
/// </summary>
/// <param name="device_">デバイス</param>
/// <returns>生成されたポインタ</returns>
std::unique_ptr<IFence> CreateUniqueFence(ID3D12Device* device_);

/// <summary>
/// フェンスの生成(シェアード)
/// </summary>
/// <param name="device_">デバイス</param>
/// <returns>生成されたポインタ</returns>
std::shared_ptr<IFence> CreateSharedFence(ID3D12Device* device_);
