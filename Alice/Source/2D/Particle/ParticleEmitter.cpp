#include<ParticleEmitter.h>

void ParticleEmitter::SetPosition(const AliceMathF::Vector3& pos)
{
	position = pos;
}

void ParticleEmitter::SetRotation(const AliceMathF::Vector2& rotation)
{
	particleRotation = rotation;
}

void ParticleEmitter::SetIntervalTime(size_t time)
{
	maxIntervalTime = time;
}

void ParticleEmitter::SetLife(UINT time)
{
	lifeTime = time;
}

void ParticleEmitter::SetIndex(size_t index)
{
	maxIndex = index;
}

void ParticleEmitter::SetTex(uint32_t handle)
{
	textureData = TextureManager::GetTextureData(handle);
}

void ParticleEmitter::SetVelocity(const AliceMathF::Vector3& velocity)
{
	particleVelocity = velocity;
}

void ParticleEmitter::SetScale(const AliceMathF::Vector2& scale)
{
	particleScale = scale;
}

void ParticleEmitter::SetStartColor(const AliceMathF::Vector4& color)
{
	startColor = color;
}

void ParticleEmitter::SetEndColor(const AliceMathF::Vector4& color)
{
	endColor = color;
}

void ParticleEmitter::SetAccel(const AliceMathF::Vector3& accel)
{
	particleAccel = accel;
}

void ParticleEmitter::Initialize()
{
	particle.reset(Particle::CreateParticle());
}

void ParticleEmitter::FireWorkUpdate()
{
	//角度の最小単位
	float angleDiv = AliceMathF::AX_2PI / float(maxIndex);

	if (intervalTime <= 0)
	{
		for (size_t i = 0; i < maxIndex; i++)
		{
			AliceMathF::Vector3 vel{};
			//基準の速度で円形状に分布
			vel.x = static_cast<float>(AliceMathF::Cos(angleDiv * static_cast<float>(i)) * (particleVelocity.x / 2));
			vel.y = static_cast<float>(AliceMathF::Sin(angleDiv * static_cast<float>(i)) * (particleVelocity.y / 2));
			vel.z = 0.0f;

			//基準の加速度でランダムに分布
			AliceMathF::Vector3 acc{};
			acc.y = static_cast<float>(rand()) / RAND_MAX * particleAccel.y;

			particle->Add(lifeTime, position, vel, acc, particleScale, particleRotation, startColor, endColor);
		}

		intervalTime = maxIntervalTime;
	}
	else
	{
		intervalTime--;
	}

	particle->Update();
}

void ParticleEmitter::Draw(Camera* camera, Material* material)
{
	particle->Draw(camera, material);
}

void ParticleEmitter::BasicUpdate()
{
	for (size_t i = 0; i < maxIndex; i++)
	{
		//基準の位置でランダムに分布
		AliceMathF::Vector3 pos{};
		pos.x = static_cast<float>(rand()) / RAND_MAX * position.x - position.x / 2.0f;
		pos.y = static_cast<float>(rand()) / RAND_MAX * position.y - position.y / 2.0f;
		pos.z = static_cast<float>(rand()) / RAND_MAX * position.z - position.z / 2.0f;

		//基準の速度でランダムに分布
		AliceMathF::Vector3 vel{};
		vel.x = static_cast<float>(rand()) / RAND_MAX * particleVelocity.x - particleVelocity.x / 2.0f;
		vel.y = static_cast<float>(rand()) / RAND_MAX * particleVelocity.y - particleVelocity.y / 2.0f;
		vel.z = static_cast<float>(rand()) / RAND_MAX * particleVelocity.z - particleVelocity.z / 2.0f;

		//基準の加速度でランダムに分布

		AliceMathF::Vector3 acc{};
		acc.y = static_cast<float>(rand()) / RAND_MAX * particleAccel.y;

		//生存時間
		UINT life = lifeTime * rand() / RAND_MAX;

		particle->Add(life, pos, vel, acc, particleScale, particleRotation, startColor, endColor);
	}
}
