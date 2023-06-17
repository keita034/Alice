#include <Particle.h>

TextureManager* Particle::sTextureManager = nullptr;
ID3D12GraphicsCommandList* Particle::sCmdList = nullptr;
ID3D12Device* Particle::sDevice = nullptr;

Particle::Particle()
{
}

Particle::~Particle()
{
}

void Particle::SetTex(uint32_t handle_)
{
	textureData = sTextureManager->SGetTextureData(handle_);
}

void Particle::SSetTextureManager(TextureManager* textureManager_)
{
	sTextureManager = textureManager_;
}

void Particle::SSetDirectX12Core(DirectX12Core* directX12Core_)
{
	sDevice = directX12Core_->GetDevice();
	sCmdList = directX12Core_->GetCommandList();
}