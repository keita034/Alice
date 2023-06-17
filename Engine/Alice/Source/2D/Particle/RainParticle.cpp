#include<RainParticle.h>

class RainParticle : public Particle, public IRainParticle
{
private:

	AliceMathF::Matrix4 mTrans, mRot, mScale;

	AliceMathF::Vector2 particleScale = { 2.0f,0.0 };

	AliceMathF::Vector2 particleRotation = { 0.0f,0.0 };

public:

	RainParticle() = default;
	~RainParticle();

	//初期化
	virtual void Initialize()override;

	///<summary>
	///更新
	///</summary>
	virtual void Update(const AliceMathF::Vector3& centerPos_, const AliceMathF::Vector2& emitRadius_, const AliceMathF::Vector4& col_, uint32_t lifeTime_);

	///<summary>
	///ビルボード描画
	///</summary>
	virtual void Draw(Camera* camera_, Material* material_ = nullptr);

private:

	virtual void Update()override;

	/// <summary>
	/// パーティクルの追加
	/// </summary>
	/// <param name="life">生存時間</param>
	/// <param name="position">初期座標</param>
	/// <param name="velocity">速度</param>
	/// <param name="accel">加速度</param>
	/// <param name="scale">{開始時スケール,終了時スケール}</param>
	/// <param name="rotation">{開始時回転角,終了時回転角}</param>
	/// <param name="sColor">開始カラー</param>
	/// <param name="eColor">終了カラー</param>
	virtual void Add(
		uint32_t life_,
		const AliceMathF::Vector3& position_,
		const AliceMathF::Vector3& velocity_,
		const AliceMathF::Vector3& accel_,
		const AliceMathF::Vector2& scale_,
		const AliceMathF::Vector2& rotation_,
		const AliceMathF::Vector4& sColor_,
		const AliceMathF::Vector4& eColor_)override;

	//コピーコンストラクタ・代入演算子削除
	RainParticle& operator=(const RainParticle&) = delete;
	RainParticle(const RainParticle&) = delete;
};

RainParticle::~RainParticle()
{
}

void RainParticle::Initialize()
{
	//頂点バッファ生成
	vertexBuffer = CreateUniqueVertexBuffer(VERTEX_COUNT, sizeof(VerPosColScaRot));

	//定数バッファ生成
	constBuffTransform = CreateUniqueConstantBuffer(sizeof(ParticleConstBuffData));
}

void RainParticle::Update(const AliceMathF::Vector3& centerPos_, const AliceMathF::Vector2& emitRadius_, const AliceMathF::Vector4& col_, uint32_t lifeTime_)
{
	for (size_t i = 0; i < 100; i++)
	{
		//基準の位置でランダムに分布
		AliceMathF::Vector3 lPos{};
		lPos.x = AliceMathF::GetRand(centerPos_.x - emitRadius_.x, centerPos_.x + emitRadius_.x);
		lPos.y = centerPos_.y;
		lPos.z = AliceMathF::GetRand(centerPos_.y - emitRadius_.y, centerPos_.y + emitRadius_.y);

		//基準の速度でランダムに分布
		AliceMathF::Vector3 lVel{};
		lVel.x = 0.0f;
		lVel.y = -0.5f;
		lVel.z = 0.0f;

		//基準の加速度でランダムに分布

		AliceMathF::Vector3 lAcc{};
		lAcc.y = static_cast<float>(rand()) / RAND_MAX * 0.001f;

		//生存時間
		uint32_t lLife = lifeTime_ * rand() / RAND_MAX;

		Add(lLife, lPos, lVel, lAcc, particleScale, particleRotation, col_, col_);
	}

	Update();
}

void RainParticle::Update()
{
		//寿命が尽きたパーティクルを全削除
	particleDatas.remove_if([](ParticleData& x)
		{
			return x.frame >= x.numFrame;
		});

	std::forward_list<ParticleData>::iterator lIterator = particleDatas.begin();
	//全パーティクル更新
	for (; lIterator != particleDatas.end(); lIterator++)
	{
		//経過フレーム数をカウント
		lIterator->frame++;
		//速度に加速度を加算
		lIterator->velocity = lIterator->velocity + lIterator->accel;
		//速度による移動
		lIterator->position = lIterator->position + lIterator->velocity;
		//進行度を0～1の範囲に換算
		float lFrame = static_cast<float>(lIterator->frame) / static_cast<float>(lIterator->numFrame);

		//スケールの線形補間
		lIterator->scale = AliceMathF::Lerp(lIterator->sScale, lIterator->eScale, lFrame);

		//回転角の線形補間
		lIterator->rotation = AliceMathF::Lerp(lIterator->sRotation, lIterator->eRotation, lFrame);

		//カラーの線形補間
		lIterator->color = AliceMathF::Lerp(lIterator->sColor, lIterator->eColor, lFrame);
	}

	//頂点バッファへデータ転送
	VerPosColScaRot* lVertMap = nullptr;
	result = vertexBuffer->GetResource()->Map(0, nullptr, (void**)&lVertMap);
	assert(SUCCEEDED(result));
	uint16_t lCount = 0;

	lIterator = particleDatas.begin();
	for (;lIterator != particleDatas.end();lIterator++)
	{
		if (lCount == VERTEX_COUNT)
		{
			break;
		}

		//座標
		lVertMap->pos = lIterator->position;
		lVertMap->scale = lIterator->scale;
		lVertMap->rotation = lIterator->rotation;
		lVertMap->color = lIterator->color;
		//次の頂点へ
		lVertMap++;
		lCount++;
	}

	vertexBuffer->GetResource()->Unmap(0, nullptr);
}

void RainParticle::Add(
	uint32_t life_,
	const AliceMathF::Vector3& position_,
	const AliceMathF::Vector3& velocity_,
	const AliceMathF::Vector3& accel_,
	const AliceMathF::Vector2& scale_,
	const AliceMathF::Vector2& rotation_,
	const AliceMathF::Vector4& sColor_,
	const AliceMathF::Vector4& eColor_)
{
	if (std::distance(particleDatas.begin(), particleDatas.end()) < VERTEX_COUNT)
	{
		//リストに要素を追加
		particleDatas.emplace_front();
		//追加した要素の参照
		ParticleData& lParticleData = particleDatas.front();
		//値のセット
		lParticleData.position = position_;
		lParticleData.velocity = velocity_;
		lParticleData.accel = accel_;
		lParticleData.numFrame = life_;
		lParticleData.sScale = scale_.x;
		lParticleData.eScale = scale_.y;
		lParticleData.scale = scale_.x;
		lParticleData.sRotation = rotation_.x;
		lParticleData.eRotation = rotation_.y;
		lParticleData.rotation = rotation_.x;
		lParticleData.sColor = sColor_;
		lParticleData.eColor = eColor_;
		lParticleData.color = sColor_;
	}
}

void RainParticle::Draw(Camera* camera_, Material* material_)
{
	D3D12_VERTEX_BUFFER_VIEW lVbView = vertexBuffer->GetView();

	assert(camera_);

	if (!material_)
	{
		particleMaterial = MaterialManager::SGetMaterial("DefaultRainParticle");
	}

	AliceMathF::Matrix4 lMat = camera_->GetViewMatrix();
	lMat.m[3][0] = 0;
	lMat.m[3][1] = 0;
	lMat.m[3][2] = 0;
	lMat.m[3][3] = 1;

	constMapTransform.matBillboard = lMat;
	constMapTransform.mat = camera_->GetViewMatrixInv() * camera_->GetProjectionMatrix();
	constBuffTransform->Update(&constMapTransform);

	// パイプラインステートとルートシグネチャの設定コマンド
	sCmdList->SetPipelineState(particleMaterial->pipelineState->GetPipelineState());
	sCmdList->SetGraphicsRootSignature(particleMaterial->rootSignature->GetRootSignature());

	// プリミティブ形状の設定コマンド
	sCmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_POINTLIST); // 三角形リスト

	// 頂点バッファビューの設定コマンド
	sCmdList->IASetVertexBuffers(0, 1, &lVbView);

	// 定数バッファビュー(CBV)の設定コマンド
	sCmdList->SetGraphicsRootConstantBufferView(0, constBuffTransform->GetAddress());

	// SRVヒープの設定コマンド
	ID3D12DescriptorHeap* lDescriptorHeaps[] = { textureData->srvHeap };
	sCmdList->SetDescriptorHeaps(1, lDescriptorHeaps);

	// SRVヒープの先頭にあるSRVをルートパラメータ1番に設定
	sCmdList->SetGraphicsRootDescriptorTable(1, textureData->gpuHandle);

	// 描画コマンド
	if (std::distance(particleDatas.begin(), particleDatas.end()) < VERTEX_COUNT)
	{
		sCmdList->DrawInstanced(static_cast<UINT> (std::distance(particleDatas.begin(), particleDatas.end())), 1, 0, 0);
	}
	else
	{
		sCmdList->DrawInstanced(VERTEX_COUNT, 1, 0, 0);
	}
}

std::unique_ptr<IRainParticle> CreateUniqueRainParticle()
{
	std::unique_ptr<IRainParticle> lRainParticle = std::make_unique<RainParticle>();
	lRainParticle->Initialize();
	return std::move(lRainParticle);
}

std::shared_ptr<IRainParticle> CreateSharedRainParticle()
{
	std::shared_ptr<IRainParticle> lRainParticle = std::make_shared<RainParticle>();
	lRainParticle->Initialize();
	return lRainParticle;
}
