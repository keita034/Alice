#pragma once

#pragma warning(push)
#pragma warning(disable: 4061)
#pragma warning(disable: 4365)
#pragma warning(disable: 4514)
#pragma warning(disable: 4668)
#pragma warning(disable: 4820)
#pragma warning(disable: 5039)

#include<array>
#include<cassert>
#include<dxgi1_6.h>
#include<d3dx12.h>

#pragma warning(pop)

#include"BaseBuffer.h"

class RenderTargetBuffer : public BaseBuffer
{
	//幅
	UINT width = 0;
	//高さ
	UINT height = 0;
	//ハンドル
	D3D12_CPU_DESCRIPTOR_HANDLE handle{};
	//ステータス
	D3D12_RESOURCE_STATES states{};
	char PADING[4];

public:

	/// <summary>
	/// 生成
	/// </summary>
	/// <param name="w">横幅</param>
	/// <param name="h">縦幅</param>
	/// <param name="resourceStates">ステータス</param>
	/// <param name="mipLevel">ミップレベル</param>
	/// <param name="arraySize">配列のサイズ</param>
	/// <param name="format">フォーマット</param>
	/// <param name="clearColor">カラー</param>
	bool Create(UINT w, UINT h, D3D12_RESOURCE_STATES resourceStates, UINT16 mipLevel = 0, UINT16 arraySize = 1, DXGI_FORMAT format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB, const std::array<float, 4>& clearColor = { {1.0f,1.0f,1.0f,1.0f } });

	/// <summary>
	/// 生成
	/// </summary>
	/// <param name="swapChain4"></param>
	/// <returns></returns>
	bool Create(IDXGISwapChain4* swapChain,UINT index);

	/// <summary>
	/// リソースを取得
	/// </summary>
	ID3D12Resource* GetTexture() const;

	/// <summary>
	/// ポインタ番号を取得
	/// </summary>
	const D3D12_CPU_DESCRIPTOR_HANDLE& GetHandle();

	/// <summary>
	/// リソースステータス変更
	/// </summary>
	/// <param name="resourceStates">ステータス</param>
	void Transition(D3D12_RESOURCE_STATES resourceStates);

	/// <summary>
	/// リセット
	/// </summary>
	void Reset();

	void Resize(IDXGISwapChain4* swapChain, UINT index);

	~RenderTargetBuffer() = default;
	RenderTargetBuffer() = default;
};

