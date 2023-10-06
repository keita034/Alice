#pragma once
#include<BaseGPUParticle.h>
#include<Color.h>

class BasicGPUParticle : public BaseGPUParticle
{
private:

	struct IndirectCommand
	{
		AliceMathF::Matrix4  worldMatrix;
		D3D12_DRAW_ARGUMENTS drawArguments;
	};

	static std::unordered_map<uint32_t,AliceMathF::Matrix4> positions;

public:

	BasicGPUParticle() = default;
	~BasicGPUParticle() = default;

	void Initialize() override;
	void Update(float deltaTime_) override;
	void Finalize() override;
	void Draw() override;

private:

	void PUpdateConstantBuffer(float deltaTime_)override;

	//コピーコンストラクタ・代入演算子削除
	BasicGPUParticle& operator=(const BasicGPUParticle&) = delete;
	BasicGPUParticle(const BasicGPUParticle&) = delete;
};

	