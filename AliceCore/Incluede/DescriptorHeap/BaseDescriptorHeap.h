#pragma once
#pragma warning(push)
#pragma warning(disable: 4061)
#pragma warning(disable: 4062)
#pragma warning(disable: 4265)
#pragma warning(disable: 4365)
#pragma warning(disable: 4625)
#pragma warning(disable: 4626)
#pragma warning(disable: 4668)
#pragma warning(disable: 4820)
#pragma warning(disable: 5039)
#pragma warning(disable: 5204)
#pragma warning(disable: 5220)
#pragma warning(disable: 4514)

#include<cassert>
#include<directx/d3d12.h>
#include<wrl.h>
#pragma warning(pop)

class BaseDescriptorHeap
{
protected:

	//デバイス
	static ID3D12Device* device;
	//デスクプリタヒープ
	Microsoft::WRL::ComPtr <ID3D12DescriptorHeap> descriptorHeap;

public:

	static void SetDevice(ID3D12Device* dev);
};

