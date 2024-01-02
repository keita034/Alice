#pragma once

#include<Defined.h>
#include<PadingType.h>

ALICE_SUPPRESS_WARNINGS_BEGIN

#include<directx/d3d12.h>
#include<wrl.h>

ALICE_SUPPRESS_WARNINGS_END

#include<MultiAdapters.h>

class GPUTimeStamp
{
private:

	Microsoft::WRL::ComPtr<ID3D12QueryHeap>mainHeap;
	Microsoft::WRL::ComPtr<ID3D12Resource>mainResource;

	Microsoft::WRL::ComPtr<ID3D12QueryHeap>subHeap;
	Microsoft::WRL::ComPtr<ID3D12Resource>subResource;

	constexpr static uint32_t FRAME_COUNT = 4;
	constexpr static uint32_t QUERY_COUNT = 2;

	uint32_t  offset = 0;
	Byte4 PADING;

public:

	void Initialize(IMultiAdapters* adapters_);
	void Update();
	void Finalize();
	void Draw();

};

