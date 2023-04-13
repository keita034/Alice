#include "Particle.h"
#include"BasicParticle.h"
#include"RainParticle.h"
#include"TextureManager.h"

Particle::Particle()
{
}

Particle::~Particle()
{
}

BasicParticle* Particle::CreateParticle()
{
	BasicParticle* basic = new BasicParticle;
	basic->Initialize();
	return basic;
}

RainParticle* Particle::CreateRainParticle()
{
	RainParticle* rain = new RainParticle;
	rain->Initialize();
	return rain;
}

void Particle::SetTex(uint32_t handle)
{
	textureData = TextureManager::GetTextureData(handle);
}
