#pragma once
#include"ErrorException.h"
#include"AliceUtility.h"
#include"RenderTargetBuffer.h"
#include"DepthStencilBuffer.h"
#include"DescriptorHeap.h"
class RenderTarget
{
private:

	//レンダーターゲットテクスチャ
	std::unique_ptr<RenderTargetBuffer>renderTargetBuffer;
	//デプステクスチャ
	std::unique_ptr<DepthStencilBuffer>depthStencilBuffer;
	//コマンドリスト
	ID3D12GraphicsCommandList* cmdList;
	//クリアーカラー
	std::array<float, 4> clearColor;

	DescriptorHeap* srvHeap = nullptr;

	D3D12_GPU_DESCRIPTOR_HANDLE handle;

public:

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(UINT w, UINT h, D3D12_RESOURCE_STATES resourceStates, const AliceMathF::Vector4& col = { 1.0f,1.0f, 1.0f, 1.0f }, DXGI_FORMAT rtFormat = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB, DXGI_FORMAT dsFormat = DXGI_FORMAT_D32_FLOAT);

	/// <summary>
	/// レンダーターゲットとデプスステンシルをセット＆クリアー
	/// </summary>
	void Begin();

	/// <summary>
	/// レンダーターゲットとデプスステンシルをセット
	/// </summary>
	void SetRenderTarget();

	/// <summary>
	/// レンダーターゲットとデプスステンシルをクリアー
	/// </summary>
	void ClearRenderTarget();

	/// <summary>
	/// 
	/// </summary>
	void Transition(D3D12_RESOURCE_STATES resourceStates);

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="d3dDevice">デバイス</param>
	/// <param name="rtvDescriptorHeap">レンダーターゲットテクスチャ用のデスクプリタヒープ</param>
	/// <param name="descriptorHeap">デプステクスチャ用のデスクプリタヒープ</param>
	/// <param name="commandList">コマンドリスト</param>
	RenderTarget(DescriptorHeap* srvDescriptorHeap, ID3D12GraphicsCommandList* commandList);

	/// <summary>
	/// レンダーターゲットテクスチャを取得
	/// </summary>
	RenderTargetBuffer* GetRenderTargetBuffer();

	/// <summary>
	/// デプステクスチャを取得
	/// </summary>
	DepthStencilBuffer* GetDepthStencilBuffer();

	const D3D12_GPU_DESCRIPTOR_HANDLE& GetGpuHandle();

	const D3D12_CPU_DESCRIPTOR_HANDLE& GetCpuHandle();

private:

	RenderTarget() = delete;
};

