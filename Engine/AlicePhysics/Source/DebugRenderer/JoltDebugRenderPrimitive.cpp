
#include "JoltDebugRenderPrimitive.h"

void AlicePhysics::JoltDebugRenderPrimitive::CreateVertexBuffer(size_t numVtx_, size_t vtxSize_, const void* data_)
{
	vertexBuffer = CreateUniqueVertexBuffer(numVtx_, vtxSize_, data_);
	vertexDrawCount = numVtx_;
}

void AlicePhysics::JoltDebugRenderPrimitive::CreateIndexBuffer(size_t numIdx_, const uint32_t* data_)
{
	indexBuffer = CreateUniqueIndexBuffer(numIdx_, data_);
	indexDrawCount = numIdx_;
}

void AlicePhysics::JoltDebugRenderPrimitive::CreateConstantBuffer(size_t vtxSize_, void* data_)
{
	constantBuffer = CreateUniqueConstantBuffer(vtxSize_);
	constantBuffer->Update(data_);
}

void AlicePhysics::JoltDebugRenderPrimitive::VertexUpdate(void* data_, size_t length_)
{
	vertexBuffer->Update(data_, length_);
}

void AlicePhysics::JoltDebugRenderPrimitive::IndexUpdate(uint32_t* data_, size_t length_)
{
	indexBuffer->Update(data_, length_);
}

void AlicePhysics::JoltDebugRenderPrimitive::ConstantUpdate(void* data_)
{
	constantBuffer->Update(data_);
}

void AlicePhysics::JoltDebugRenderPrimitive::Draw()
{
	ID3D12GraphicsCommandList* lGraphicsCommandList = commandList->GetGraphicCommandList();

	// プリミティブ形状の設定コマンド
	lGraphicsCommandList->IASetPrimitiveTopology(type); //三角形リスト

	D3D12_VERTEX_BUFFER_VIEW lVbView = vertexBuffer->GetView();

	// 頂点バッファビューの設定コマンド
	lGraphicsCommandList->IASetVertexBuffers(0, 1, &lVbView);

	if (indexBuffer)
	{
		D3D12_INDEX_BUFFER_VIEW libView = indexBuffer->GetView();

		// 頂点バッファビューの設定コマンド
		lGraphicsCommandList->IASetIndexBuffer(&libView);

		// 描画コマンド
		lGraphicsCommandList->DrawIndexedInstanced(static_cast<UINT>(indexDrawCount), 1, 0, 0, 0);
	}
	else
	{
		// 描画コマンド
		lGraphicsCommandList->DrawInstanced(static_cast<UINT>(vertexDrawCount), 1, 0, 0);
	}

}

void AlicePhysics::JoltDebugRenderPrimitive::GeometryDraw()
{
	ID3D12GraphicsCommandList* lGraphicsCommandList = commandList->GetGraphicCommandList();

// プリミティブ形状の設定コマンド
	lGraphicsCommandList->IASetPrimitiveTopology(type); //三角形リスト

	D3D12_VERTEX_BUFFER_VIEW lVbView = vertexBuffer->GetView();

	// 頂点バッファビューの設定コマンド
	lGraphicsCommandList->IASetVertexBuffers(0, 1, &lVbView);

	if (indexBuffer)
	{
		D3D12_INDEX_BUFFER_VIEW libView = indexBuffer->GetView();

		// 頂点バッファビューの設定コマンド
		lGraphicsCommandList->IASetIndexBuffer(&libView);

		lGraphicsCommandList->SetGraphicsRootConstantBufferView(0, constantBuffer->GetAddress());

		// 描画コマンド
		lGraphicsCommandList->DrawIndexedInstanced(static_cast<UINT>(indexDrawCount), 1, 0, 0, 0);
	}
	else
	{
		lGraphicsCommandList->SetGraphicsRootConstantBufferView(0, constantBuffer->GetAddress());

		// 描画コマンド
		lGraphicsCommandList->DrawInstanced(static_cast<UINT>(vertexDrawCount), 1, 0, 0);
	}

}

void AlicePhysics::JoltDebugRenderPrimitive::GeometryOFFDraw()
{
	ID3D12GraphicsCommandList* lGraphicsCommandList = commandList->GetGraphicCommandList();

	// プリミティブ形状の設定コマンド
	lGraphicsCommandList->IASetPrimitiveTopology(type); //三角形リスト

	D3D12_VERTEX_BUFFER_VIEW lVbView = vertexBuffer->GetView();

	// 頂点バッファビューの設定コマンド
	lGraphicsCommandList->IASetVertexBuffers(0, 1, &lVbView);

	if (indexBuffer)
	{
		D3D12_INDEX_BUFFER_VIEW libView = indexBuffer->GetView();

		// 頂点バッファビューの設定コマンド
		lGraphicsCommandList->IASetIndexBuffer(&libView);

		// 描画コマンド
		lGraphicsCommandList->DrawIndexedInstanced(static_cast<UINT>(indexDrawCount), 1, 0, 0, 0);
	}
	else
	{

		// 描画コマンド
		lGraphicsCommandList->DrawInstanced(static_cast<UINT>(vertexDrawCount), 1, 0, 0);
	}
}


AlicePhysics::JoltDebugRenderPrimitive::JoltDebugRenderPrimitive(ICommandList* commandList_, D3D_PRIMITIVE_TOPOLOGY type_)
	: commandList(commandList_), type(type_) { }
