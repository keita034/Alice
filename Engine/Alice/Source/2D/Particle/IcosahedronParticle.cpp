#include<IcosahedronParticle.h>

#include<Particle.h>
#include<DefaultMaterial.h>

class IcosahedronParticle : public Particle, public IIcosahedronParticle
{
private:

	AliceMathF::Matrix4 mTrans, mRot, mScale;

public:

	IcosahedronParticle() = default;
	~IcosahedronParticle();

	//初期化
	virtual void Initialize()override;

	///<summary>
	///更新
	///</summary>
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

	///<summary>
	///ビルボード描画
	///</summary>
	virtual void Draw(Camera* camera_, const Material* material_ = nullptr)override;

private:

	//コピーコンストラクタ・代入演算子削除
	IcosahedronParticle& operator=(const IcosahedronParticle&) = delete;
	IcosahedronParticle(const IcosahedronParticle&) = delete;
};


IcosahedronParticle::~IcosahedronParticle()
{
}

void IcosahedronParticle::Initialize()
{
	//頂点バッファ生成
	vertexBuffer = CreateUniqueVertexBuffer(VERTEX_COUNT, sizeof(VerPosColScaRot));

	//定数バッファ生成
	constBuffTransform = CreateUniqueConstantBuffer(sizeof(ParticleConstBuffData));
}

void IcosahedronParticle::Update()
{
	//寿命が尽きたパーティクルを全削除
	particleDatas.remove_if([](ParticleData& p)
		{
			return p.frame >= p.numFrame;
		});

	std::forward_list<ParticleData>::iterator lIterator = particleDatas.begin();

	//全パーティクル更新
	for (;lIterator != particleDatas.end();lIterator++)
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

void IcosahedronParticle::Add(
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

void IcosahedronParticle::Draw(Camera* camera_, const Material* material_)
{
	D3D12_VERTEX_BUFFER_VIEW lVbView = vertexBuffer->GetView();
	ID3D12GraphicsCommandList* lCmdList = sCmdList->GetGraphicCommandList();

	assert(camera_);

	if (!material_)
	{
		particleMaterial = MaterialManager::SGetMaterial("IcosahedronParticle");
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
	lCmdList->SetPipelineState(particleMaterial->pipelineState->GetPipelineState());
	lCmdList->SetGraphicsRootSignature(particleMaterial->rootSignature->GetRootSignature());

	// プリミティブ形状の設定コマンド
	lCmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_POINTLIST); // 三角形リスト

	// 頂点バッファビューの設定コマンド
	lCmdList->IASetVertexBuffers(0, 1, &lVbView);

	// 定数バッファビュー(CBV)の設定コマンド
	lCmdList->SetGraphicsRootConstantBufferView(0, constBuffTransform->GetAddress());

	// SRVヒープの設定コマンド
	ID3D12DescriptorHeap* lDescriptorHeaps[] = { textureData->srvHeap };
	lCmdList->SetDescriptorHeaps(1, lDescriptorHeaps);

	// SRVヒープの先頭にあるSRVをルートパラメータ1番に設定
	lCmdList->SetGraphicsRootDescriptorTable(1, textureData->gpuHandle);

	// 描画コマンド
	if (std::distance(particleDatas.begin(), particleDatas.end()) < VERTEX_COUNT)
	{
		lCmdList->DrawInstanced(static_cast<UINT> (std::distance(particleDatas.begin(), particleDatas.end())), 1, 0, 0);
	}
	else
	{
		lCmdList->DrawInstanced(VERTEX_COUNT, 1, 0, 0);
	}
}

std::unique_ptr<IIcosahedronParticle> CreateUniqueIcosahedronParticle()
{
	std::unique_ptr<IIcosahedronParticle> lIcosahedronParticle = std::make_unique<IcosahedronParticle>();
	lIcosahedronParticle->Initialize();

	return std::move(lIcosahedronParticle);
}

std::shared_ptr<IIcosahedronParticle> CreateSharedIcosahedronParticle()
{
	std::shared_ptr<IIcosahedronParticle> lIcosahedronParticle = std::make_shared<IcosahedronParticle>();
	lIcosahedronParticle->Initialize();
	return lIcosahedronParticle;
}
