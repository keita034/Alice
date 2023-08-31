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
	/// <param name="queue_">キュー</param>
	virtual void WaitPreviousFrame(ID3D12CommandQueue* queue_) = 0;
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
