#pragma once

#include"DescriptorHeap.h"
#include"RTVDescriptorHeap.h"
#include"DSVDescriptorHeap.h"

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

#include<wrl.h>
#pragma warning(pop)

class BaseBuffer
{
protected:

	//デバイス
	static ID3D12Device* sDevice;
	//コマンドリスト
	static ID3D12GraphicsCommandList* sCommandList;
	//SRV用のディスクプリタヒープ
	static IDescriptorHeap* sSRVHeap;
	//RTV用ディスクプリタヒープ
	static IRTVDescriptorHeap* sRTVHeap;
	//DSV用のディスクプリタヒープ
	static IDSVDescriptorHeap* sDSVHeap;

	//リソース
	Microsoft::WRL::ComPtr<ID3D12Resource> resource;

	//成功したか
	bool isValid = false;

private:
	int8_t PADING[7]{};

public:

	/// <summary>
	/// デバイスを設定
	/// </summary>
	static void SSetDevice(ID3D12Device* device_);

	/// <summary>
	/// コマンドリストを設定
	/// </summary>
	static void SSetGraphicsCommandList(ID3D12GraphicsCommandList* commandList_);

	/// <summary>
	/// SRV用のディスクプリタヒープを設定
	/// </summary>
	static void SSetSRVDescriptorHeap(IDescriptorHeap* descriptorHeap_);

	/// <summary>
	/// RTV用のディスクプリタヒープを設定
	/// </summary>
	static void SSetRTVDescriptorHeap(IRTVDescriptorHeap* rtvDescriptorHeap_);

	/// <summary>
	/// DSV用のディスクプリタヒープを設定
	/// </summary>
	static void SSetDSVDescriptorHeap(IDSVDescriptorHeap* dsvDescriptorHeap_);
};