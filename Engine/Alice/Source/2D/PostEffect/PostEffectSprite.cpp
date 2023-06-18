#include<PostEffectSprite.h>

void PostEffectSprite::Initialize(ID3D12GraphicsCommandList* commandList_, IDescriptorHeap* descriptorHeap_)
{
	cmdList = commandList_;
	srvHeap = descriptorHeap_;

	//頂点バッファの生成
	vertexBuffer = CreateUniqueVertexBuffer(4, sizeof(PosUvColor));

	//インデックスバッファの生成
	indexBuffer = CreateUniqueIndexBuffer(6);
}

void PostEffectSprite::SetSize(const AliceMathF::Vector2& size_)
{
	spriteSize = size_;
}

void PostEffectSprite::Draw(const Material* material_, D3D12_GPU_DESCRIPTOR_HANDLE handle_)
{
	// 頂点データ
	PosUvColor lVertices[] =
	{//		x		y		z		u	v
		{{-spriteSize.x,spriteSize.y,0.0f},{0.0f,0.0f},{1.0f,1.0f,1.0f,1.0f}},//左上インデックス0
		{{-spriteSize.x,-spriteSize.y,0.0f},{0.0f,1.0f},{1.0f,1.0f,1.0f,1.0f}},//左下インデックス1
		{{spriteSize.x,spriteSize.y,0.0f},{1.0f,0.0f},{1.0f,1.0f,1.0f,1.0f}},//右上インデックス2
		{{spriteSize.x,-spriteSize.y,0.0f},{1.0f,1.0f},{1.0f,1.0f,1.0f,1.0f}},//右下インデックス3
	};

	// インデックスデータ
	uint32_t lIndices[] =
	{
		1, 0, 3, // 三角形1つ目
		2, 3, 0, // 三角形2つ目
	};

	//頂点バッファへのデータ転送
	vertexBuffer->Update(lVertices);

	//インデックスバッファへのデータ転送
	indexBuffer->Update(lIndices);

	D3D12_VERTEX_BUFFER_VIEW lVbView = vertexBuffer->GetView();
	D3D12_INDEX_BUFFER_VIEW lIbView = indexBuffer->GetView();

	// パイプラインステートとルートシグネチャの設定コマンド
	cmdList->SetPipelineState(material_->pipelineState->GetPipelineState());
	cmdList->SetGraphicsRootSignature(material_->rootSignature->GetRootSignature());

	// プリミティブ形状の設定コマンド
	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST); // 三角形リスト

	// 頂点バッファビューの設定コマンド
	cmdList->IASetVertexBuffers(0, 1, &lVbView);

	//インデックスバッファビューの設定コマンド
	cmdList->IASetIndexBuffer(&lIbView);

	// SRVヒープの設定コマンド
	ID3D12DescriptorHeap* lDescriptorHeaps[] =
	{
		srvHeap->GetHeap(),
	};

	cmdList->SetDescriptorHeaps(_countof(lDescriptorHeaps), lDescriptorHeaps);

	//// SRVヒープの先頭にあるSRVをルートパラメータ1番に設定
	cmdList->SetGraphicsRootDescriptorTable(0, handle_);

}
