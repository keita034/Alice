#include "Device.h"

ALICE_SUPPRESS_WARNINGS_BEGIN

#include<wrl.h>
#include<directx/d3d12.h>
#include<dxgi1_6.h>
#include<cassert>

ALICE_SUPPRESS_WARNINGS_END

class Device : public IDevice
{
private:

	//デバイス
	Microsoft::WRL::ComPtr<ID3D12Device> device;

public:

	Device() = default;
	~Device() = default;

	void Initialize(IDXGIAdapter* adapter_) override;

	ID3D12Device* Get()override ;

	ID3D12Device** GetDeviceAddressOf()override ;
};


void Device::Initialize(IDXGIAdapter* adapter_)
{
		//対応レベルの配列
	D3D_FEATURE_LEVEL lLevels[ ] =
	{
		D3D_FEATURE_LEVEL_12_1,
		D3D_FEATURE_LEVEL_12_0,
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
	};

	HRESULT lResult;
	for ( size_t i = 0; i < _countof(lLevels); i++ )
	{
		// 採用したアダプターでデバイスを生成
		lResult = D3D12CreateDevice(adapter_,lLevels[ i ],IID_PPV_ARGS(device.ReleaseAndGetAddressOf()));
		if (SUCCEEDED(lResult) )
		{
			// デバイスを生成できた時点でループを抜ける
			break;
		}
	}
}

ID3D12Device* Device::Get()
{
    return device.Get();
}

ID3D12Device** Device::GetDeviceAddressOf()
{
	return device.GetAddressOf();
}

std::unique_ptr<IDevice> CreateUniqueDevice(IDXGIAdapter* adapter_)
{
	assert(adapter_);
	std::unique_ptr<IDevice> lDevice = std::make_unique<Device>();
	lDevice->Initialize(adapter_);
	return std::move(lDevice);
}

std::shared_ptr<IDevice> CreateSharedDevice(IDXGIAdapter* adapter_)
{
	assert(adapter_);
	std::shared_ptr<IDevice> lDevice = std::make_shared<Device>();
	lDevice->Initialize(adapter_);
	return lDevice;
}
