#pragma once

#include<VertexBuffer.h>
#include<IndexBuffer.h>
#include<Material.h>
#include<AliceMathUtility.h>
#include<DescriptorHeap.h>
#include<PosUvColor.h>

class PostEffectSprite
{
private:

	//頂点バッファ
	std::unique_ptr<IVertexBuffer> vertexBuffer;

	//インデックスバッファ
	std::unique_ptr<IIndexBuffer> indexBuffer;

	ID3D12GraphicsCommandList* cmdList = nullptr;

	ISRVDescriptorHeap* srvHeap = nullptr;

	AliceMathF::Vector2 spriteSize;

public:

	PostEffectSprite() = default;
	~PostEffectSprite() = default;

	void Initialize(ID3D12GraphicsCommandList* commandList_, ISRVDescriptorHeap* descriptorHeap_);

	void SetSize(const AliceMathF::Vector2& size_);

	void Draw(const Material* material_, D3D12_GPU_DESCRIPTOR_HANDLE handle_);

private:

	PostEffectSprite(const PostEffectSprite&) = delete;
	PostEffectSprite& operator=(const PostEffectSprite&) = delete;
};

