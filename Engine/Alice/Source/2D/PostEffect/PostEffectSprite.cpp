#include<PostEffectSprite.h>

void PostEffectSprite::Initialize(ID3D12GraphicsCommandList* commandList, DescriptorHeap* descriptorHeap)
{
	cmdList = commandList;
	srvHeap = descriptorHeap;

	//頂点バッファの生成
	vertexBuffer = std::make_unique<VertexBuffer>();
	vertexBuffer->Create(4, sizeof(PosUvColor));

	//インデックスバッファの生成
	indexBuffer = std::make_unique<IndexBuffer>();
	indexBuffer->Create(6);
}

void PostEffectSprite::SetSize(const AliceMathF::Vector2& size)
{
	spriteSize = size;
}

void PostEffectSprite::Draw(Material* material, D3D12_GPU_DESCRIPTOR_HANDLE handle)
{
	// 頂点データ
	PosUvColor vertices[] =
	{//		x		y		z		u	v
		{{-spriteSize.x,spriteSize.y,0.0f},{0.0f,0.0f},{1.0f,1.0f,1.0f,1.0f}},//左上インデックス0
		{{-spriteSize.x,-spriteSize.y,0.0f},{0.0f,1.0f},{1.0f,1.0f,1.0f,1.0f}},//左下インデックス1
		{{spriteSize.x,spriteSize.y,0.0f},{1.0f,0.0f},{1.0f,1.0f,1.0f,1.0f}},//右上インデックス2
		{{spriteSize.x,-spriteSize.y,0.0f},{1.0f,1.0f},{1.0f,1.0f,1.0f,1.0f}},//右下インデックス3
	};

	// インデックスデータ
	uint32_t indices[] =
	{
		1, 0, 3, // 三角形1つ目
		2, 3, 0, // 三角形2つ目
	};

	//頂点バッファへのデータ転送
	vertexBuffer->Update(vertices);

	//インデックスバッファへのデータ転送
	indexBuffer->Update(indices);

	D3D12_VERTEX_BUFFER_VIEW vbView = vertexBuffer->GetView();
	D3D12_INDEX_BUFFER_VIEW ibView = indexBuffer->GetView();

	// パイプラインステートとルートシグネチャの設定コマンド
	cmdList->SetPipelineState(material->pipelineState->GetPipelineState());
	cmdList->SetGraphicsRootSignature(material->rootSignature->GetRootSignature());

	// プリミティブ形状の設定コマンド
	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST); // 三角形リスト

	// 頂点バッファビューの設定コマンド
	cmdList->IASetVertexBuffers(0, 1, &vbView);

	//インデックスバッファビューの設定コマンド
	cmdList->IASetIndexBuffer(&ibView);

	// SRVヒープの設定コマンド
	ID3D12DescriptorHeap* descriptorHeaps[] =
	{
		srvHeap->GetHeap(),
	};

	cmdList->SetDescriptorHeaps(_countof(descriptorHeaps), descriptorHeaps);

	//// SRVヒープの先頭にあるSRVをルートパラメータ1番に設定
	cmdList->SetGraphicsRootDescriptorTable(0, handle);

}
