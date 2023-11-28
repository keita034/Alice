#pragma once
#include<AliceUtility.h>
#include<RenderTargetBuffer.h>
#include<DepthStencilBuffer.h>
#include<DescriptorHeap.h>

class RenderTarget
{
private:

	//レンダーターゲットテクスチャ
	std::unique_ptr<IRenderTargetBuffer>renderTargetBuffer;
	//デプステクスチャ
	std::unique_ptr<IDepthStencilBuffer>depthStencilBuffer;
	//コマンドリスト
	ID3D12GraphicsCommandList* cmdList;
	//クリアーカラー
	std::array<float, 4> clearColor = {};

	ISRVDescriptorHeap* srvHeap = nullptr;

	D3D12_GPU_DESCRIPTOR_HANDLE handle = {};

public:

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(
		uint32_t width_, 
		uint32_t height_,
		D3D12_RESOURCE_STATES resourceStates_,
		const AliceMathF::Vector4& clear_ = { 0.1f, 0.25f, 0.5f, 1.0f },
		DXGI_FORMAT rtFormat_ = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB,
		DXGI_FORMAT dsFormat_ = DXGI_FORMAT_D32_FLOAT);

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

	void Clear();

	/// <summary>
	/// 
	/// </summary>
	void Transition(D3D12_RESOURCE_STATES resourceStates_);

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="d3dDevice">デバイス</param>
	/// <param name="rtvDescriptorHeap">レンダーターゲットテクスチャ用のデスクプリタヒープ</param>
	/// <param name="descriptorHeap">デプステクスチャ用のデスクプリタヒープ</param>
	/// <param name="commandList">コマンドリスト</param>
	RenderTarget(ISRVDescriptorHeap* srvDescriptorHeap_, ID3D12GraphicsCommandList* commandList_);

	/// <summary>
	/// レンダーターゲットテクスチャを取得
	/// </summary>
	IRenderTargetBuffer* GetRenderTargetBuffer()const;

	/// <summary>
	/// デプステクスチャを取得
	/// </summary>
	IDepthStencilBuffer* GetDepthStencilBuffer()const;

	const D3D12_GPU_DESCRIPTOR_HANDLE& GetGpuHandle()const;

	const D3D12_CPU_DESCRIPTOR_HANDLE& GetCpuHandle()const;

private:

	RenderTarget() = delete;
		//コピーコンストラクタ・代入演算子削除
	RenderTarget& operator=(const RenderTarget&) = delete;
	RenderTarget(const RenderTarget&) = delete;
};

