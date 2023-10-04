#pragma once

#include<DescriptorHeap.h>
#include<RTVDescriptorHeap.h>
#include<DSVDescriptorHeap.h>
#include<Device.h>
#include<CommandList.h>
#include<Defined.h>

ALICE_SUPPRESS_WARNINGS_BEGIN

#include<wrl.h>
ALICE_SUPPRESS_WARNINGS_END

class BaseBuffer
{
protected:

	//デバイス
	static IDevice* sDevice;
	//コマンドリスト
	static ICommandList* sCommandList;
	//SRV用のディスクプリタヒープ
	static ISRVDescriptorHeap* sSRVHeap;
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
	static void SSetDevice(IDevice* device_);

	/// <summary>
	/// コマンドリストを設定
	/// </summary>
	static void SSetGraphicsCommandList(ICommandList* commandList_);

	/// <summary>
	/// SRV用のディスクプリタヒープを設定
	/// </summary>
	static void SSetSRVDescriptorHeap(ISRVDescriptorHeap* descriptorHeap_);

	/// <summary>
	/// RTV用のディスクプリタヒープを設定
	/// </summary>
	static void SSetRTVDescriptorHeap(IRTVDescriptorHeap* rtvDescriptorHeap_);

	/// <summary>
	/// DSV用のディスクプリタヒープを設定
	/// </summary>
	static void SSetDSVDescriptorHeap(IDSVDescriptorHeap* dsvDescriptorHeap_);

	/// <summary>
	/// デバイスを取得
	/// </summary>
	static IDevice* SGetDevice();

	/// <summary>
	/// コマンドリストを取得
	/// </summary>
	static ICommandList* SGetGraphicsCommandList();

	/// <summary>
	/// SRV用のディスクプリタヒープを取得
	/// </summary>
	static ISRVDescriptorHeap* SGetSRVDescriptorHeap();

	/// <summary>
	/// RTV用のディスクプリタヒープを取得
	/// </summary>
	static IRTVDescriptorHeap* SGetRTVDescriptorHeap();

	/// <summary>
	/// DSV用のディスクプリタヒープを取得
	/// </summary>
	static IDSVDescriptorHeap* SGetDSVDescriptorHeap();

	static void Finalize();
};