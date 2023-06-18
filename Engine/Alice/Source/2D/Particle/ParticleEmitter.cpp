#include<ParticleEmitter.h>
#include<BasicParticle.h>

class ParticleEmitter : public IParticleEmitter
{
private:

	//基準の座標
	AliceMathF::Vector3 position = { 0,0,0 };
	//開始時カラー
	AliceMathF::Vector4 startColor = { 1.0f,0.0f,0.0f,1.0f };
	//終了時カラー
	AliceMathF::Vector4 endColor = { 0.0f,0.0f,1.0f,1.0f };
	//基準の加速度
	AliceMathF::Vector3 particleAccel = { 0.0f, 0.001f ,0.0f };
	//基準の速度
	AliceMathF::Vector3 particleVelocity = { 0.1f,0.1f,0.1f };
	//スケール(開始時スケール,終了時スケール)
	AliceMathF::Vector2 particleScale = { 1.0f,0.0f };
	//回転角(開始時回転角,終了時回転角)
	AliceMathF::Vector2 particleRotation = { 0.0f,0.0f };

	//基準の生存時間
	uint32_t lifeTime = 50;
	//最大発生間隔
	size_t maxIntervalTime = 60;
	//発生間隔
	size_t intervalTime = 0;
	//一度に発生させる数
	size_t maxIndex = 50;

	//パーティクル
	std::unique_ptr<IBasicParticle> particle;

	TextureData* textureData;

public:

	/// <summary>
	/// 基準の座標を設定
	/// </summary>
	/// <param name="pos">座標</param>
	void SetPosition(const AliceMathF::Vector3& pos_) override;

	/// <summary>
	/// 開始時カラーを設定
	/// </summary>
	/// <param name="color">カラー</param>
	void SetStartColor(const AliceMathF::Vector4& color_)override;

	/// <summary>
	/// 終了時カラーを設定
	/// </summary>
	/// <param name="color">カラー</param>
	void SetEndColor(const AliceMathF::Vector4& color_)override;

	/// <summary>
	/// 基準の加速度を設定
	/// </summary>
	/// <param name="accel">加速度</param>
	void SetAccel(const AliceMathF::Vector3& accel_)override;

	/// <summary>
	/// 基準の速度を設定
	/// </summary>
	/// <param name="velocity">速度</param>
	void SetVelocity(const AliceMathF::Vector3& velocity_)override;

	/// <summary>
	/// スケールを設定
	/// </summary>
	/// <param name="scale">(開始時スケール,終了時スケール)</param>
	void SetScale(const AliceMathF::Vector2& scale_)override;

	/// <summary>
	/// 回転角を設定
	/// </summary>
	/// <param name="rotation">(開始時回転角,終了時回転角)</param>
	void SetRotation(const AliceMathF::Vector2& rotation_)override;

	/// <summary>
	/// 発生間隔を設定
	/// </summary>
	/// <param name="time">発生間隔</param>
	void SetIntervalTime(size_t time_)override;

	/// <summary>
	/// 基準の生存時間を設定
	/// </summary>
	/// <param name="time">生存時間</param>
	void SetLife(uint32_t time_)override;

	/// <summary>
	/// 一度に発生させる数を設定
	/// </summary>
	/// <param name="index">一度に発生させる数</param>
	void SetIndex(size_t index_)override;

	
	void SetTex(uint32_t handle_)override;

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize()override;

	/// <summary>
	/// 更新処理
	/// </summary>
	void FireWorkUpdate()override;

	void BasicUpdate()override;

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(Camera* camera_, const Material* material_ = nullptr);

	ParticleEmitter() = default;
	~ParticleEmitter() = default;

private:

	//コピーコンストラクタ・代入演算子削除
	ParticleEmitter& operator=(const ParticleEmitter&) = delete;
	ParticleEmitter(const ParticleEmitter&) = delete;
};

void ParticleEmitter::SetPosition(const AliceMathF::Vector3& pos_)
{
	position = pos_;
}

void ParticleEmitter::SetRotation(const AliceMathF::Vector2& rotation_)
{
	particleRotation = rotation_;
}

void ParticleEmitter::SetIntervalTime(size_t time_)
{
	maxIntervalTime = time_;
}

void ParticleEmitter::SetLife(uint32_t time_)
{
	lifeTime = time_;
}

void ParticleEmitter::SetIndex(size_t index_)
{
	maxIndex = index_;
}

void ParticleEmitter::SetTex(uint32_t handle_)
{
	textureData = TextureManager::SGetTextureData(handle_);
}

void ParticleEmitter::SetVelocity(const AliceMathF::Vector3& velocity_)
{
	particleVelocity = velocity_;
}

void ParticleEmitter::SetScale(const AliceMathF::Vector2& scale_)
{
	particleScale = scale_;
}

void ParticleEmitter::SetStartColor(const AliceMathF::Vector4& color_)
{
	startColor = color_;
}

void ParticleEmitter::SetEndColor(const AliceMathF::Vector4& color_)
{
	endColor = color_;
}

void ParticleEmitter::SetAccel(const AliceMathF::Vector3& accel_)
{
	particleAccel = accel_;
}

void ParticleEmitter::Initialize()
{
	particle = CreateUniqueBasicParticle();
}

void ParticleEmitter::FireWorkUpdate()
{
	//角度の最小単位
	float lAngleDiv = AliceMathF::AX_2PI / float(maxIndex);

	if (intervalTime <= 0)
	{
		for (size_t i = 0; i < maxIndex; i++)
		{
			AliceMathF::Vector3 lVel{};
			//基準の速度で円形状に分布
			lVel.x = static_cast<float>(AliceMathF::Cos(lAngleDiv * static_cast<float>(i)) * (particleVelocity.x / 2));
			lVel.y = static_cast<float>(AliceMathF::Sin(lAngleDiv * static_cast<float>(i)) * (particleVelocity.y / 2));
			lVel.z = 0.0f;

			//基準の加速度でランダムに分布
			AliceMathF::Vector3 acc{};
			acc.y = static_cast<float>(rand()) / RAND_MAX * particleAccel.y;

			particle->Add(lifeTime, position, lVel, acc, particleScale, particleRotation, startColor, endColor);
		}

		intervalTime = maxIntervalTime;
	}
	else
	{
		intervalTime--;
	}

	particle->Update();
}

void ParticleEmitter::Draw(Camera* camera_, const Material* material_)
{
	particle->Draw(camera_, material_);
}

void ParticleEmitter::BasicUpdate()
{
	for (size_t i = 0; i < maxIndex; i++)
	{
		//基準の位置でランダムに分布
		AliceMathF::Vector3 lPos{};
		lPos.x = static_cast<float>(rand()) / RAND_MAX * position.x - position.x / 2.0f;
		lPos.y = static_cast<float>(rand()) / RAND_MAX * position.y - position.y / 2.0f;
		lPos.z = static_cast<float>(rand()) / RAND_MAX * position.z - position.z / 2.0f;

		//基準の速度でランダムに分布
		AliceMathF::Vector3 lVel{};
		lVel.x = static_cast<float>(rand()) / RAND_MAX * particleVelocity.x - particleVelocity.x / 2.0f;
		lVel.y = static_cast<float>(rand()) / RAND_MAX * particleVelocity.y - particleVelocity.y / 2.0f;
		lVel.z = static_cast<float>(rand()) / RAND_MAX * particleVelocity.z - particleVelocity.z / 2.0f;

		//基準の加速度でランダムに分布

		AliceMathF::Vector3 lAcc{};
		lAcc.y = static_cast<float>(rand()) / RAND_MAX * particleAccel.y;

		//生存時間
		uint32_t lLife = lifeTime * rand() / RAND_MAX;

		particle->Add(lLife, lPos, lVel, lAcc, particleScale, particleRotation, startColor, endColor);
	}
}

std::unique_ptr<IParticleEmitter> CreateUniqueParticleEmitter()
{
	std::unique_ptr<IParticleEmitter> lParticleEmitter = std::make_unique<ParticleEmitter>();
	lParticleEmitter->Initialize();
	return std::move(lParticleEmitter);
}

std::shared_ptr<IParticleEmitter> CreateSharedParticleEmitter()
{
	std::shared_ptr<IParticleEmitter> lParticleEmitter = std::make_shared<ParticleEmitter>();
	lParticleEmitter->Initialize();
	return lParticleEmitter;
}
