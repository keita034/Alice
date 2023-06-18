#include<Sprite.h>
#include<DirectX12Core.h>

IWindowsApp* Sprite::sWindowsApp = nullptr;
ID3D12Device* Sprite::sDevice = nullptr;
ID3D12GraphicsCommandList* Sprite::sCmdList = nullptr;

void Sprite::PSpriteDraw(const Transform& transform_, const Material* material_)
{
	D3D12_VERTEX_BUFFER_VIEW lVbView = vertexBuffer->GetView();
	D3D12_INDEX_BUFFER_VIEW lIbView = indexBuffer->GetView();

	// パイプラインステートとルートシグネチャの設定コマンド
	sCmdList->SetPipelineState(material_->pipelineState->GetPipelineState());
	sCmdList->SetGraphicsRootSignature(material_->rootSignature->GetRootSignature());

	// プリミティブ形状の設定コマンド
	sCmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST); // 三角形リスト

	// 頂点バッファビューの設定コマンド
	sCmdList->IASetVertexBuffers(0, 1, &lVbView);

	//インデックスバッファビューの設定コマンド
	sCmdList->IASetIndexBuffer(&lIbView);

	// 定数バッファビュー(CBV)の設定コマンド
	sCmdList->SetGraphicsRootConstantBufferView(0, transform_.GetAddress());

	// SRVヒープの設定コマンド
	ID3D12DescriptorHeap* lDescriptorHeaps[] = { texture->srvHeap };
	sCmdList->SetDescriptorHeaps(1, lDescriptorHeaps);

	//// SRVヒープの先頭にあるSRVをルートパラメータ1番に設定
	sCmdList->SetGraphicsRootDescriptorTable(1, texture->gpuHandle);

	// 描画コマンド
	sCmdList->DrawIndexedInstanced(6, 1, 0, 0, 0);
}

void Sprite::PCreatVertexIndexBuffer()
{
	//頂点バッファの生成
	vertexBuffer = CreateUniqueVertexBuffer(4, sizeof(PosUvColor));

	//インデックスバッファの生成
	indexBuffer = CreateUniqueIndexBuffer(6);
}

Sprite::Sprite()
{
}

Sprite::~Sprite()
{
}

void Sprite::SetAnchorPoint(const AliceMathF::Vector2& point_)
{
	anchorPoint = point_;
}

void Sprite::SetFlipFlag(bool isFlipX_, bool isFlipY_)
{
	flipX = isFlipX_;
	flipY = isFlipY_;
}

void Sprite::SetColor(const AliceMathF::Vector4& color_)
{
	color = color_;
}

void Sprite::SetTextureTrimmingRange(const AliceMathF::Vector2& leftTop_, const AliceMathF::Vector2& rightDown_)
{
	trimmingRange = { leftTop_.x,leftTop_.y,rightDown_.x,rightDown_.y };
}

void Sprite::SetTex(uint32_t handle_)
{
	texture = TextureManager::SGetTextureData(handle_);
	trimmingRange.z = static_cast<float>(texture->width);
	trimmingRange.w = static_cast<float>(texture->height);
	spriteSize.x = static_cast<float>(texture->width);
	spriteSize.y = static_cast<float>(texture->height);
}

void Sprite::SetSize(const AliceMathF::Vector2& size_)
{
	spriteSize.x = size_.x;
	spriteSize.y = size_.y;
}

const TextureData* Sprite::GetTexture()
{
	return texture;
}

void Sprite::SSetWindowsApp(IWindowsApp* windowsApp_)
{
	sWindowsApp = windowsApp_;
}

void Sprite::SSetDirectX12Core(DirectX12Core* directX12Core_)
{
	sDevice = directX12Core_->GetDevice();
	sCmdList = directX12Core_->GetCommandList();
}
