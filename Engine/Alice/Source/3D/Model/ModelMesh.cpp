#include<ModelMesh.h>
#include<atldef.h>
ModelMesh::~ModelMesh()
{
}
void ModelMesh::CreateBuffer()
{
	//頂点バッファ生成
	//全体のサイズ計算
	size_t lSize = vertices.size();
	//一つ分のサイズ計算
	size_t lSingleSize = sizeof(PosNormUvTangeColSkin);
	//生成
	vertexBuffer = CreateUniqueVertexBuffer(lSize, lSingleSize);
	if (!vertexBuffer->IsValid())
	{
		printf("Failed to create vertex buffer\n");
		return;
	}

	//インデックスバッファ生成
	//全体のサイズ計算
	lSize = indices.size();
	uint32_t* lIndice = indices.data();
	//生成
	indexBuffer = CreateUniqueIndexBuffer(lSize, lIndice);
	if (!indexBuffer->IsValid())
	{
		printf("Failed to create index buffer\n");
		return;
	}

	materialBuffer = CreateUniqueConstantBuffer(sizeof(ConstBuffDataMaterial));
	if (!materialBuffer->IsValid())
	{
		printf("Failed to create constant buffer for material\n");
		return;
	}

	constBoneBuffer = CreateUniqueConstantBuffer(sizeof(BoneData));
	if (!constBoneBuffer->IsValid())
	{
		printf("Failed to generate constant buffer for bone data\n");
		return;
	}
	constBoneBuffer->Update(bonedata.boneMat.data());

	// 変換行列の登録
	ConstBuffDataMaterial lmaterial = { material.ambient,0.0f,material.diffuse,0.0f,material.specular,0.0f,material.emission,0.0f,material.shininess,material.alpha };
	materialBuffer->Update(&lmaterial);

	vertexBuffer->Update(vertices.data());
}

const std::vector<PosNormUvTangeColSkin>& ModelMesh::GetVertices()
{
	return vertices;
}

const std::vector<uint32_t>& ModelMesh::GetIndices()
{
	return indices;
}

void ModelMesh::InitializeVertex()
{
	vertexBuffer->Update(vertices.data());
}

void ModelMesh::Draw(ID3D12GraphicsCommandList* cmdList_, const Transform& transform_, const Light* light_)
{
	D3D12_VERTEX_BUFFER_VIEW lVbView = vertexBuffer->GetView();
	D3D12_INDEX_BUFFER_VIEW lIbView = indexBuffer->GetView();

	for (TextureData* texture : textures)
	{
		// 定数バッファビュー(CBV)の設定コマンド
		cmdList_->SetGraphicsRootConstantBufferView(0, transform_.GetAddress());
		cmdList_->SetGraphicsRootConstantBufferView(1, materialBuffer->GetAddress());
		light_->SetConstBufferView(cmdList_, 2);
		// 頂点バッファビューの設定コマンド
		cmdList_->IASetVertexBuffers(0, 1, &lVbView);

		//インデックスバッファビューの設定コマンド
		cmdList_->IASetIndexBuffer(&lIbView);

		// SRVヒープの設定コマンド
		ID3D12DescriptorHeap* descriptorHeaps_[] = { texture->srvHeap };
		cmdList_->SetDescriptorHeaps(1, descriptorHeaps_);

		// SRVヒープの先頭にあるSRVをルートパラメータ2番に設定
		cmdList_->SetGraphicsRootDescriptorTable(4, texture->gpuHandle);

		// 描画コマンド
		cmdList_->DrawIndexedInstanced(static_cast<UINT>(indices.size()), 1, 0, 0, 0);

	}
}

void ModelMesh::AnimDraw(ID3D12GraphicsCommandList* cmdList_, const Transform& transform_, const Light* light_)
{
	D3D12_VERTEX_BUFFER_VIEW lVbView = vertexBuffer->GetView();
	D3D12_INDEX_BUFFER_VIEW lIbView = indexBuffer->GetView();

	for (TextureData* texture : textures)
	{
		// 定数バッファビュー(CBV)の設定コマンド
		cmdList_->SetGraphicsRootConstantBufferView(0, transform_.GetAddress());
		cmdList_->SetGraphicsRootConstantBufferView(1, materialBuffer->GetAddress());
		light_->SetConstBufferView(cmdList_, 2);
		cmdList_->SetGraphicsRootConstantBufferView(3, constBoneBuffer->GetAddress());

		// 頂点バッファビューの設定コマンド
		cmdList_->IASetVertexBuffers(0, 1, &lVbView);

		//インデックスバッファビューの設定コマンド
		cmdList_->IASetIndexBuffer(&lIbView);

		// SRVヒープの設定コマンド
		ID3D12DescriptorHeap* lDescriptorHeaps[] = { texture->srvHeap };
		cmdList_->SetDescriptorHeaps(1, lDescriptorHeaps);

		// SRVヒープの先頭にあるSRVをルートパラメータ2番に設定
		cmdList_->SetGraphicsRootDescriptorTable(4, texture->gpuHandle);

		// 描画コマンド
		cmdList_->DrawIndexedInstanced(static_cast<UINT>(indices.size()), 1, 0, 0, 0);

	}
}

void ModelMesh::ToonDraw(ID3D12GraphicsCommandList* cmdList_, const Transform& transform_, D3D12_GPU_DESCRIPTOR_HANDLE rampHandle_, const Light* light_)
{
	D3D12_VERTEX_BUFFER_VIEW lVbView = vertexBuffer->GetView();
	D3D12_INDEX_BUFFER_VIEW lIbView = indexBuffer->GetView();

	for (TextureData* texture : textures)
	{
		// 定数バッファビュー(CBV)の設定コマンド
		cmdList_->SetGraphicsRootConstantBufferView(0, transform_.GetAddress());
		cmdList_->SetGraphicsRootConstantBufferView(1, materialBuffer->GetAddress());
		light_->SetConstBufferView(cmdList_, 2);
		// 頂点バッファビューの設定コマンド
		cmdList_->IASetVertexBuffers(0, 1, &lVbView);

		//インデックスバッファビューの設定コマンド
		cmdList_->IASetIndexBuffer(&lIbView);

		// SRVヒープの設定コマンド
		ID3D12DescriptorHeap* lDescriptorHeaps[] = { texture->srvHeap };
		cmdList_->SetDescriptorHeaps(1, lDescriptorHeaps);

		// SRVヒープの先頭にあるSRVをルートパラメータ2番に設定
		cmdList_->SetGraphicsRootDescriptorTable(4, texture->gpuHandle);
		cmdList_->SetGraphicsRootDescriptorTable(5, rampHandle_);

		// 描画コマンド
		cmdList_->DrawIndexedInstanced(static_cast<UINT>(indices.size()), 1, 0, 0, 0);
	}
}

void ModelMesh::OutLineDraw(ID3D12GraphicsCommandList* cmdList_, const Transform& transform_)
{
	D3D12_VERTEX_BUFFER_VIEW lVbView = vertexBuffer->GetView();
	D3D12_INDEX_BUFFER_VIEW lIbView = indexBuffer->GetView();

	// 定数バッファビュー(CBV)の設定コマンド
	cmdList_->SetGraphicsRootConstantBufferView(0, transform_.GetAddress());

	// 頂点バッファビューの設定コマンド
	cmdList_->IASetVertexBuffers(0, 1, &lVbView);

	//インデックスバッファビューの設定コマンド
	cmdList_->IASetIndexBuffer(&lIbView);

	// 描画コマンド
	cmdList_->DrawIndexedInstanced(static_cast<UINT>(indices.size()), 1, 0, 0, 0);
}

void ModelMesh::AnimToonDraw(ID3D12GraphicsCommandList* cmdList_, const Transform& transform_, D3D12_GPU_DESCRIPTOR_HANDLE rampHandle_, const Light* light_)
{
	D3D12_VERTEX_BUFFER_VIEW lVbView = vertexBuffer->GetView();
	D3D12_INDEX_BUFFER_VIEW lIbView = indexBuffer->GetView();

	for (TextureData* texture : textures)
	{
		// 定数バッファビュー(CBV)の設定コマンド
		cmdList_->SetGraphicsRootConstantBufferView(0, transform_.GetAddress());
		cmdList_->SetGraphicsRootConstantBufferView(1, materialBuffer->GetAddress());
		light_->SetConstBufferView(cmdList_, 2);
		cmdList_->SetGraphicsRootConstantBufferView(3, constBoneBuffer->GetAddress());

		// 頂点バッファビューの設定コマンド
		cmdList_->IASetVertexBuffers(0, 1, &lVbView);

		//インデックスバッファビューの設定コマンド
		cmdList_->IASetIndexBuffer(&lIbView);

		// SRVヒープの設定コマンド
		ID3D12DescriptorHeap* lDescriptorHeaps_[] = { texture->srvHeap };
		cmdList_->SetDescriptorHeaps(1, lDescriptorHeaps_);

		// SRVヒープの先頭にあるSRVをルートパラメータ2番に設定
		cmdList_->SetGraphicsRootDescriptorTable(4, texture->gpuHandle);
		cmdList_->SetGraphicsRootDescriptorTable(5, rampHandle_);

		// 描画コマンド
		cmdList_->DrawIndexedInstanced(static_cast<UINT>(indices.size()), 1, 0, 0, 0);

	}
}

void ModelMesh::AnimOutLineDraw(ID3D12GraphicsCommandList* cmdList_, const Transform& transform_)
{
	D3D12_VERTEX_BUFFER_VIEW lVbView = vertexBuffer->GetView();
	D3D12_INDEX_BUFFER_VIEW lIbView = indexBuffer->GetView();

	// 定数バッファビュー(CBV)の設定コマンド
	cmdList_->SetGraphicsRootConstantBufferView(0, transform_.GetAddress());
	cmdList_->SetGraphicsRootConstantBufferView(1, constBoneBuffer->GetAddress());

	// 頂点バッファビューの設定コマンド
	cmdList_->IASetVertexBuffers(0, 1, &lVbView);

	//インデックスバッファビューの設定コマンド
	cmdList_->IASetIndexBuffer(&lIbView);
	// 描画コマンド
	cmdList_->DrawIndexedInstanced(static_cast<UINT>(indices.size()), 1, 0, 0, 0);
}

void ModelMesh::Update()
{
	for (size_t i = 0; i < vecBones.size(); i++)
	{
		bonedata.boneMat[i] = vecBones[i].matrix;
	}

	constBoneBuffer->Update(bonedata.boneMat.data());
}