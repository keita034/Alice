#pragma once
#include"DescriptorHeap.h"
#include"RTVDescriptorHeap.h"
#include"DSVDescriptorHeap.h"

class BaseBuffer
{
protected:

	//デバイス
	static ID3D12Device* device;
	static ID3D12GraphicsCommandList* commandList;

	static DescriptorHeap* SRVHeap;
	static RTVDescriptorHeap* RTVHeap;
	static DSVDescriptorHeap* DSVHeap;

	//リソース
	Microsoft::WRL::ComPtr<ID3D12Resource> resource;

	//成功したか
	bool isValid = false;

private:
	char PADING[7]{};

public:

	static void SetDevice(ID3D12Device* dev);
	static void SetGraphicsCommandList(ID3D12GraphicsCommandList* cmdList);
	static void SetSRVDescriptorHeap(DescriptorHeap* srv);
	static void SetRTVDescriptorHeap(RTVDescriptorHeap* rtv);
	static void SetDSVDescriptorHeap(DSVDescriptorHeap* dsv);
};

