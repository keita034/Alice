#include <Particle.h>

TextureManager* Particle::sTextureManager = nullptr;
ICommandList* Particle::sCmdList = nullptr;
IDevice* Particle::sMainDevice = nullptr;

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
	sMainDevice = directX12Core_->GetDevice();
	sCmdList = directX12Core_->GetCommandList();
}