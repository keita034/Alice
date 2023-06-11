#include<ModelMesh.h>
#include<atldef.h>
ModelMesh::~ModelMesh()
{
}
void ModelMesh::CreateBuffer()
{
	//頂点バッファ生成
	//全体のサイズ計算
	size_t size = vertices.size();
	//一つ分のサイズ計算
	size_t stride = sizeof(PosNormUvTangeColSkin);
	//生成
	vertexBuffer = CreateUniqueVertexBuffer(size, stride);
	if (!vertexBuffer->IsValid())
	{
		printf("Failed to create vertex buffer\n");
		return;
	}

	//インデックスバッファ生成
	//全体のサイズ計算
	size = indices.size();
	uint32_t* indice = indices.data();
	//生成
	indexBuffer = CreateUniqueIndexBuffer(size, indice);
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
	ConstBuffDataMaterial mate = { material.ambient,0.0f,material.diffuse,0.0f,material.specular,0.0f,material.emission,0.0f,material.shininess,material.alpha };
	materialBuffer->Update(&mate);

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

void ModelMesh::Draw(ID3D12GraphicsCommandList* cmdList, Transform& transform, Light* light)
{
	D3D12_VERTEX_BUFFER_VIEW vbView = vertexBuffer->GetView();
	D3D12_INDEX_BUFFER_VIEW ibView = indexBuffer->GetView();

	for (TextureData* texture : textures)
	{
		// 定数バッファビュー(CBV)の設定コマンド
		cmdList->SetGraphicsRootConstantBufferView(0, transform.GetAddress());
		cmdList->SetGraphicsRootConstantBufferView(1, materialBuffer->GetAddress());
		light->SetConstBufferView(cmdList, 2);
		// 頂点バッファビューの設定コマンド
		cmdList->IASetVertexBuffers(0, 1, &vbView);

		//インデックスバッファビューの設定コマンド
		cmdList->IASetIndexBuffer(&ibView);

		// SRVヒープの設定コマンド
		ID3D12DescriptorHeap* descriptorHeaps[] = { texture->srvHeap };
		cmdList->SetDescriptorHeaps(1, descriptorHeaps);

		// SRVヒープの先頭にあるSRVをルートパラメータ2番に設定
		cmdList->SetGraphicsRootDescriptorTable(4, texture->gpuHandle);

		// 描画コマンド
		cmdList->DrawIndexedInstanced(static_cast<UINT>(indices.size()), 1, 0, 0, 0);

	}
}

void ModelMesh::AnimDraw(ID3D12GraphicsCommandList* cmdList, Transform& transform, Light* light)
{
	D3D12_VERTEX_BUFFER_VIEW vbView = vertexBuffer->GetView();
	D3D12_INDEX_BUFFER_VIEW ibView = indexBuffer->GetView();

	for (TextureData* texture : textures)
	{
		// 定数バッファビュー(CBV)の設定コマンド
		cmdList->SetGraphicsRootConstantBufferView(0, transform.GetAddress());
		cmdList->SetGraphicsRootConstantBufferView(1, materialBuffer->GetAddress());
		light->SetConstBufferView(cmdList, 2);
		cmdList->SetGraphicsRootConstantBufferView(3, constBoneBuffer->GetAddress());

		// 頂点バッファビューの設定コマンド
		cmdList->IASetVertexBuffers(0, 1, &vbView);

		//インデックスバッファビューの設定コマンド
		cmdList->IASetIndexBuffer(&ibView);

		// SRVヒープの設定コマンド
		ID3D12DescriptorHeap* descriptorHeaps[] = { texture->srvHeap };
		cmdList->SetDescriptorHeaps(1, descriptorHeaps);

		// SRVヒープの先頭にあるSRVをルートパラメータ2番に設定
		cmdList->SetGraphicsRootDescriptorTable(4, texture->gpuHandle);

		// 描画コマンド
		cmdList->DrawIndexedInstanced(static_cast<UINT>(indices.size()), 1, 0, 0, 0);

	}
}

void ModelMesh::ToonDraw(ID3D12GraphicsCommandList* cmdList, Transform& transform, D3D12_GPU_DESCRIPTOR_HANDLE rampHandle, Light* light)
{
	D3D12_VERTEX_BUFFER_VIEW vbView = vertexBuffer->GetView();
	D3D12_INDEX_BUFFER_VIEW ibView = indexBuffer->GetView();

	for (TextureData* texture : textures)
	{
		// 定数バッファビュー(CBV)の設定コマンド
		cmdList->SetGraphicsRootConstantBufferView(0, transform.GetAddress());
		cmdList->SetGraphicsRootConstantBufferView(1, materialBuffer->GetAddress());
		light->SetConstBufferView(cmdList, 2);
		// 頂点バッファビューの設定コマンド
		cmdList->IASetVertexBuffers(0, 1, &vbView);

		//インデックスバッファビューの設定コマンド
		cmdList->IASetIndexBuffer(&ibView);

		// SRVヒープの設定コマンド
		ID3D12DescriptorHeap* descriptorHeaps[] = { texture->srvHeap };
		cmdList->SetDescriptorHeaps(1, descriptorHeaps);

		// SRVヒープの先頭にあるSRVをルートパラメータ2番に設定
		cmdList->SetGraphicsRootDescriptorTable(4, texture->gpuHandle);
		cmdList->SetGraphicsRootDescriptorTable(5, rampHandle);

		// 描画コマンド
		cmdList->DrawIndexedInstanced(static_cast<UINT>(indices.size()), 1, 0, 0, 0);
	}
}

void ModelMesh::OutLineDraw(ID3D12GraphicsCommandList* cmdList, Transform& transform)
{
	D3D12_VERTEX_BUFFER_VIEW vbView = vertexBuffer->GetView();
	D3D12_INDEX_BUFFER_VIEW ibView = indexBuffer->GetView();

	// 定数バッファビュー(CBV)の設定コマンド
	cmdList->SetGraphicsRootConstantBufferView(0, transform.GetAddress());

	// 頂点バッファビューの設定コマンド
	cmdList->IASetVertexBuffers(0, 1, &vbView);

	//インデックスバッファビューの設定コマンド
	cmdList->IASetIndexBuffer(&ibView);

	// 描画コマンド
	cmdList->DrawIndexedInstanced(static_cast<UINT>(indices.size()), 1, 0, 0, 0);
}

void ModelMesh::AnimToonDraw(ID3D12GraphicsCommandList* cmdList, Transform& transform, D3D12_GPU_DESCRIPTOR_HANDLE rampHandle, Light* light)
{
	D3D12_VERTEX_BUFFER_VIEW vbView = vertexBuffer->GetView();
	D3D12_INDEX_BUFFER_VIEW ibView = indexBuffer->GetView();

	for (TextureData* texture : textures)
	{
		// 定数バッファビュー(CBV)の設定コマンド
		cmdList->SetGraphicsRootConstantBufferView(0, transform.GetAddress());
		cmdList->SetGraphicsRootConstantBufferView(1, materialBuffer->GetAddress());
		light->SetConstBufferView(cmdList, 2);
		cmdList->SetGraphicsRootConstantBufferView(3, constBoneBuffer->GetAddress());

		// 頂点バッファビューの設定コマンド
		cmdList->IASetVertexBuffers(0, 1, &vbView);

		//インデックスバッファビューの設定コマンド
		cmdList->IASetIndexBuffer(&ibView);

		// SRVヒープの設定コマンド
		ID3D12DescriptorHeap* descriptorHeaps[] = { texture->srvHeap };
		cmdList->SetDescriptorHeaps(1, descriptorHeaps);

		// SRVヒープの先頭にあるSRVをルートパラメータ2番に設定
		cmdList->SetGraphicsRootDescriptorTable(4, texture->gpuHandle);
		cmdList->SetGraphicsRootDescriptorTable(5, rampHandle);

		// 描画コマンド
		cmdList->DrawIndexedInstanced(static_cast<UINT>(indices.size()), 1, 0, 0, 0);

	}
}

void ModelMesh::AnimOutLineDraw(ID3D12GraphicsCommandList* cmdList, Transform& transform)
{
	D3D12_VERTEX_BUFFER_VIEW vbView = vertexBuffer->GetView();
	D3D12_INDEX_BUFFER_VIEW ibView = indexBuffer->GetView();

	// 定数バッファビュー(CBV)の設定コマンド
	cmdList->SetGraphicsRootConstantBufferView(0, transform.GetAddress());
	cmdList->SetGraphicsRootConstantBufferView(1, constBoneBuffer->GetAddress());

	// 頂点バッファビューの設定コマンド
	cmdList->IASetVertexBuffers(0, 1, &vbView);

	//インデックスバッファビューの設定コマンド
	cmdList->IASetIndexBuffer(&ibView);
	// 描画コマンド
	cmdList->DrawIndexedInstanced(static_cast<UINT>(indices.size()), 1, 0, 0, 0);
}

void ModelMesh::Update()
{
	for (size_t i = 0; i < vecBones.size(); i++)
	{
		bonedata.boneMat[i] = vecBones[i].matrix;
	}

	constBoneBuffer->Update(bonedata.boneMat.data());
}