#include<RenderTarget.h>

void RenderTarget::Initialize(uint32_t width_, uint32_t height_, D3D12_RESOURCE_STATES resourceStates_, const AliceMathF::Vector4& clear_, DXGI_FORMAT rtFormat_, DXGI_FORMAT dsFormat_)
{
	//クリアカラー
	
	clearColor = {{ clear_.x,clear_.y,clear_.z,clear_.w }};
	
	//レンダーターゲットの生成
	renderTargetBuffer = CreateUniqueRenderTargetBuffer(width_, height_, resourceStates_, 0, 1, rtFormat_, clearColor);

	//デプスステンシルの生成
	depthStencilBuffer = CreateUniqueDepthStencilBuffer(width_, height_, dsFormat_);


	{//SRV作成

		D3D12_SHADER_RESOURCE_VIEW_DESC lSrvDesc{};
		lSrvDesc.Format = rtFormat_;
		lSrvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
		lSrvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
		lSrvDesc.Texture2D.MipLevels = 1;
		handle.ptr = srvHeap->CreateSRV(lSrvDesc, renderTargetBuffer->GetTexture());
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

void RenderTarget::Clear()
{
	cmdList->ClearRenderTargetView(renderTargetBuffer->GetHandle(), clearColor.data(), 0, nullptr);
}

void RenderTarget::Transition(D3D12_RESOURCE_STATES resourceStates)
{
	renderTargetBuffer->Transition(resourceStates);
}

RenderTarget::RenderTarget(IDescriptorHeap* srvDescriptorHeap_, ID3D12GraphicsCommandList* commandList_)
{

	cmdList = commandList_;

	srvHeap = srvDescriptorHeap_;
}

IRenderTargetBuffer* RenderTarget::GetRenderTargetBuffer()const
{
	return renderTargetBuffer.get();
}

IDepthStencilBuffer* RenderTarget::GetDepthStencilBuffer()const
{
	return depthStencilBuffer.get();
}

const D3D12_GPU_DESCRIPTOR_HANDLE& RenderTarget::GetGpuHandle()const
{
	return handle;
}

const D3D12_CPU_DESCRIPTOR_HANDLE& RenderTarget::GetCpuHandle()const
{
	return renderTargetBuffer->GetHandle();
}
