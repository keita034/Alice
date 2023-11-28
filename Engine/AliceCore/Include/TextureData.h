#pragma once

#include<Defined.h>
#include<PadingType.h>

ALICE_SUPPRESS_WARNINGS_BEGIN

#include<wrl.h>
#include<directx/d3d12.h>
#include<string>

ALICE_SUPPRESS_WARNINGS_END

#include<AdaptersIndex.h>

class TextureData
{
public:

	//テクスチャバッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> texBuff;

	//デスクプリタヒープ
	ID3D12DescriptorHeap* srvHeap;

	//GPUデスクプリタハンドル
	D3D12_GPU_DESCRIPTOR_HANDLE gpuHandle{};

	std::string path;

	//横幅
	size_t width = 0;
	//縦幅
	size_t height = 0;

	uint32_t textureHandle;

	AdaptersIndex index = AdaptersIndex::MAIN;

	D3D12_RESOURCE_STATES resourceState;

private:

	Byte4 PADING;

public:

	TextureData();
	~TextureData();
};