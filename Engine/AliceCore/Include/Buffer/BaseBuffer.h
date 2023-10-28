#pragma once

#include<MultiAdapters.h>
#include<CommandList.h>
#include<Defined.h>
#include<PadingType.h>
#include<SwapChain.h>

ALICE_SUPPRESS_WARNINGS_BEGIN
#include<wrl.h>
ALICE_SUPPRESS_WARNINGS_END

class BaseBuffer
{
protected:

	//デバイス
	static IMultiAdapters* sMultiAdapters;

	static ISwapChain* sSwapChain;
	//リソース
	Microsoft::WRL::ComPtr<ID3D12Resource> resource;

	//成功したか
	bool isValid = false;

private:
	Byte7 PADING;

public:

	/// <summary>
	/// デバイスを設定
	/// </summary>
	static void SSetMultiAdapters(IMultiAdapters* multiAdapters_);

	/// <summary>
	/// デバイスを設定
	/// </summary>
	static void SSetSwapChain(ISwapChain* swapChain_);

	/// <summary>
	/// デバイスを取得
	/// </summary>
	static IDevice* SGetDevice(AdaptersIndex index_);

	/// <summary>
	/// コマンドリストを取得
	/// </summary>
	static ICommandList* SGetGraphicsCommandList(AdaptersIndex index_);

	/// <summary>
	/// SRV用のディスクプリタヒープを取得
	/// </summary>
	static ISRVDescriptorHeap* SGetSRVDescriptorHeap(AdaptersIndex index_);

	/// <summary>
	/// RTV用のディスクプリタヒープを取得
	/// </summary>
	static IRTVDescriptorHeap* SGetRTVDescriptorHeap(AdaptersIndex index_);

	/// <summary>
	/// DSV用のディスクプリタヒープを取得
	/// </summary>
	static IDSVDescriptorHeap* SGetDSVDescriptorHeap(AdaptersIndex index_);

	static void Finalize();
};