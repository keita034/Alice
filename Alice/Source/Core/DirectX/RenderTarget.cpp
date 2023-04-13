#include "RenderTarget.h"

void RenderTarget::Initialize(UINT w, UINT h, D3D12_RESOURCE_STATES resourceStates, const AliceMathF::Vector4& col, DXGI_FORMAT rtFormat, DXGI_FORMAT dsFormat)
{
	//クリアカラー
	clearColor = { col.x,col.y,col.z,col.w };

	//レンダーターゲットの生成
	renderTargetBuffer->Create(w, h, resourceStates, 0, 1, rtFormat, clearColor);

	//デプスステンシルの生成
	depthStencilBuffer->Create(w, h, dsFormat);


	{//SRV作成

		D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
		srvDesc.Format = rtFormat;
		srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
		srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MipLevels = 1;
		handle.ptr = srvHeap->CreateSRV(srvDesc, renderTargetBuffer->GetTexture());
	}
}

void RenderTarget::Begin()
{
	//レンダーターゲットのセット
	cmdList->OMSetRenderTargets(1, &renderTargetBuffer->GetHandle(), false, &depthStencilBuffer->GetHandle());

	//画面クリア
	cmdList->ClearRenderTargetView(renderTargetBuffer->GetHandle(), clearColor.data(), 0, nullptr);
	cmdList->ClearDepthStencilView(depthStencilBuffer->GetHandle(), D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
}

void RenderTarget::SetRenderTarget()
{
	//レンダーターゲットのセット
	cmdList->OMSetRenderTargets(1, &renderTargetBuffer->GetHandle(), false, &depthStencilBuffer->GetHandle());
}

void RenderTarget::ClearRenderTarget()
{
	//画面クリア
	cmdList->ClearRenderTargetView(renderTargetBuffer->GetHandle(), clearColor.data(), 0, nullptr);
	cmdList->ClearDepthStencilView(depthStencilBuffer->GetHandle(), D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
}

void RenderTarget::Transition(D3D12_RESOURCE_STATES resourceStates)
{
	renderTargetBuffer->Transition(resourceStates);
}

RenderTarget::RenderTarget(DescriptorHeap* srvDescriptorHeap, ID3D12GraphicsCommandList* commandList)
{
	renderTargetBuffer = std::make_unique<RenderTargetBuffer>();

	depthStencilBuffer = std::make_unique<DepthStencilBuffer>();

	cmdList = commandList;

	srvHeap = srvDescriptorHeap;
}

RenderTargetBuffer* RenderTarget::GetRenderTargetBuffer()
{
	return renderTargetBuffer.get();
}

DepthStencilBuffer* RenderTarget::GetDepthStencilBuffer()
{
	return depthStencilBuffer.get();
}

const D3D12_GPU_DESCRIPTOR_HANDLE& RenderTarget::GetGpuHandle()
{
	return handle;
}

const D3D12_CPU_DESCRIPTOR_HANDLE& RenderTarget::GetCpuHandle()
{
	return renderTargetBuffer->GetHandle();
}
