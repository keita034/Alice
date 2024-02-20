#pragma once
#include<GPUParticleEmitter.h>


class BossReconstructionMove
{
private:

	MeshGPUParticle* meshGPUParticle = nullptr;

public:

	void Initialize(GPUParticleEmitter* emitter_);

	void Update();

	void Finalize();

	BossReconstructionMove() = default;
	~BossReconstructionMove() = default;

};

