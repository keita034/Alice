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

struct IDXGIAdapter;
struct ID3D12Device;

/// <summary>
/// デバイス
/// </summary>
class IDevice
{
public:

	IDevice() = default;
	virtual ~IDevice() = default;

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="adapter">アダプター</param>
	virtual void Initialize(IDXGIAdapter* adapter_) = 0;

	/// <summary>
	/// デバイスを取得
	/// </summary>
	virtual ID3D12Device* GetDevice() = 0;

	/// <summary>
	/// デバイスを取得
	/// </summary>
	virtual ID3D12Device** GetDeviceAddressOf() = 0;
};

/// <summary>
/// デバイスの生成(ユニーク)
/// </summary>
/// <param name="adapter">アダプター</param>
/// <returns>生成されたポインタ</returns>
std::unique_ptr<IDevice> CreateUniqueDevice(IDXGIAdapter* adapter_);

/// <summary>
/// デバイスの生成(シェアード)
/// </summary>
/// <param name="adapter">アダプター</param>
/// <returns>生成されたポインタ</returns>
std::shared_ptr<IDevice> CreateSharedDevice(IDXGIAdapter* adapter_);