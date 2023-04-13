#include "RainParticle.h"
#include"DefaultMaterial.h"

RainParticle::~RainParticle()
{
}

void RainParticle::Initialize()
{
	device = DirectX12Core::GetInstance()->GetDevice();
	cmdList = DirectX12Core::GetInstance()->GetCommandList();

	//頂点バッファ生成
	vertexBuffer = std::make_unique<VertexBuffer>();
	vertexBuffer->Create(vertexCount, sizeof(VerPosColScaRot));

	//定数バッファ生成
	constBuffTransform = std::make_unique<ConstantBuffer>();
	constBuffTransform->Create(sizeof(ParticleConstBuffData));
}

void RainParticle::Update(const AliceMathF::Vector3& centerPos, const AliceMathF::Vector2& emitRadius, const AliceMathF::Vector4& col, UINT lifeTime)
{
	for (size_t i = 0; i < 100; i++)
	{
		//基準の位置でランダムに分布
		AliceMathF::Vector3 pos{};
		pos.x = AliceMathF::GetRand(centerPos.x - emitRadius.x, centerPos.x + emitRadius.x);
		pos.y = centerPos.y;
		pos.z = AliceMathF::GetRand(centerPos.y - emitRadius.y, centerPos.y + emitRadius.y);

		//基準の速度でランダムに分布
		AliceMathF::Vector3 vel{};
		vel.x = 0.0f;
		vel.y = -0.5f;
		vel.z = 0.0f;

		//基準の加速度でランダムに分布

		AliceMathF::Vector3 acc{};
		acc.y = static_cast<float>(rand()) / RAND_MAX * 0.001f;

		//生存時間
		UINT life = lifeTime * rand() / RAND_MAX;

		Add(life, pos, vel, acc, particleScale, particleRotation, col, col);
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

	//全パーティクル更新
	for (std::forward_list<ParticleData>::iterator it = particleDatas.begin();
		it != particleDatas.end();
		it++)
	{
		//経過フレーム数をカウント
		it->frame++;
		//速度に加速度を加算
		it->velocity = it->velocity + it->accel;
		//速度による移動
		it->position = it->position + it->velocity;
		//進行度を0～1の範囲に換算
		float f = static_cast<float>(it->frame) / static_cast<float>(it->numFrame);

		//スケールの線形補間
		it->scale = AliceMathF::Lerp(it->sScale, it->eScale, f);

		//回転角の線形補間
		it->rotation = AliceMathF::Lerp(it->sRotation, it->eRotation, f);

		//カラーの線形補間
		it->color = AliceMathF::Lerp(it->sColor, it->eColor, f);
	}

	//頂点バッファへデータ転送
	VerPosColScaRot* vertMap = nullptr;
	result = vertexBuffer->GetResource()->Map(0, nullptr, (void**)&vertMap);
	assert(SUCCEEDED(result));
	uint16_t count = 0;
	for (std::forward_list<ParticleData>::iterator it = particleDatas.begin();
		it != particleDatas.end();
		it++)
	{
		if (count == vertexCount)
		{
			break;
		}

		//座標
		vertMap->pos = it->position;
		vertMap->scale = it->scale;
		vertMap->rotation = it->rotation;
		vertMap->color = it->color;
		//次の頂点へ
		vertMap++;
		count++;
	}

	vertexBuffer->GetResource()->Unmap(0, nullptr);
}

void RainParticle::Add(UINT life, const AliceMathF::Vector3& position, const AliceMathF::Vector3& velocity,
	const AliceMathF::Vector3& accel, const AliceMathF::Vector2& scale, const AliceMathF::Vector2& rotation
	, const AliceMathF::Vector4& sColor, const AliceMathF::Vector4& eColor)
{
	if (std::distance(particleDatas.begin(), particleDatas.end()) < vertexCount)
	{
		//リストに要素を追加
		particleDatas.emplace_front();
		//追加した要素の参照
		ParticleData& p = particleDatas.front();
		//値のセット
		p.position = position;
		p.velocity = velocity;
		p.accel = accel;
		p.numFrame = life;
		p.sScale = scale.x;
		p.eScale = scale.y;
		p.scale = scale.x;
		p.sRotation = rotation.x;
		p.eRotation = rotation.y;
		p.rotation = rotation.x;
		p.sColor = sColor;
		p.eColor = eColor;
		p.color = sColor;
	}
}

void RainParticle::Draw(Camera* camera, Material* material)
{
	D3D12_VERTEX_BUFFER_VIEW vbView = vertexBuffer->GetView();

	assert(camera);

	if (!material)
	{
		particleMaterial = MaterialManager::GetMaterial("DefaultRainParticle");
	}

	AliceMathF::Matrix4 mat = camera->GetViewMatrix();
	mat.m[3][0] = 0;
	mat.m[3][1] = 0;
	mat.m[3][2] = 0;
	mat.m[3][3] = 1;

	constMapTransform.matBillboard = mat;
	constMapTransform.mat = camera->GetViewMatrixInv() * camera->GetProjectionMatrix();
	constBuffTransform->Update(&constMapTransform);

	// パイプラインステートとルートシグネチャの設定コマンド
	cmdList->SetPipelineState(particleMaterial->pipelineState->GetPipelineState());
	cmdList->SetGraphicsRootSignature(particleMaterial->rootSignature->GetRootSignature());

	// プリミティブ形状の設定コマンド
	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_POINTLIST); // 三角形リスト

	// 頂点バッファビューの設定コマンド
	cmdList->IASetVertexBuffers(0, 1, &vbView);

	// 定数バッファビュー(CBV)の設定コマンド
	cmdList->SetGraphicsRootConstantBufferView(0, constBuffTransform->GetAddress());

	// 描画コマンド
	if (std::distance(particleDatas.begin(), particleDatas.end()) < vertexCount)
	{
		cmdList->DrawInstanced(static_cast<UINT> (std::distance(particleDatas.begin(), particleDatas.end())), 1, 0, 0);
	}
	else
	{
		cmdList->DrawInstanced(vertexCount, 1, 0, 0);

	}
}
