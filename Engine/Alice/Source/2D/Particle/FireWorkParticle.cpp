#include "FireWorkParticle.h"
#include<MaterialManager.h>

void FireWorkParticle::Initialize()
{
	//頂点バッファ生成
	vertexBuffer = CreateUniqueVertexBuffer(VERTEX_COUNT,sizeof(VerPosColScaRot));

	//定数バッファ生成
	constBuffTransform = CreateUniqueConstantBuffer(sizeof(ParticleConstBuffData));
}

void FireWorkParticle::Update()
{
		//寿命が尽きたパーティクルを全削除
	particleDatas.remove_if([ ] (ParticleData& x)
		{
			return x.frame >= x.numFrame;
		});

	//全パーティクル更新

	std::forward_list<ParticleData>::iterator lIterator = particleDatas.begin();

	for ( ; lIterator != particleDatas.end(); lIterator++ )
	{
		//経過フレーム数をカウント
		lIterator->frame++;
		//速度に加速度を加算
		lIterator->velocity = lIterator->velocity + lIterator->accel;
		//速度による移動
		lIterator->position = lIterator->position + lIterator->velocity;
		//進行度を0～1の範囲に換算
		float lFrame = static_cast< float >( lIterator->frame ) / static_cast< float >( lIterator->numFrame );

		//スケールの線形補間
		lIterator->scale = AliceMathF::Lerp(lIterator->sScale,lIterator->eScale,lFrame);

		//回転角の線形補間
		lIterator->rotation = AliceMathF::Lerp(lIterator->sRotation,lIterator->eRotation,lFrame);

		//カラーの線形補間
		lIterator->color = AliceMathF::Lerp(lIterator->sColor,lIterator->eColor,lFrame);
	}

	//頂点バッファへデータ転送
	VerPosColScaRot* lVertMap = nullptr;
	result = vertexBuffer->GetResource()->Map(0,nullptr,( void** ) &lVertMap);
	assert(SUCCEEDED(result));
	uint16_t lCount = 0;

	lIterator = particleDatas.begin();
	for ( ; lIterator != particleDatas.end(); lIterator++ )
	{
		if ( lCount == VERTEX_COUNT )
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

	vertexBuffer->GetResource()->Unmap(0,nullptr);
}

void FireWorkParticle::Draw(Camera* camera_,const Material* material_)
{
	D3D12_VERTEX_BUFFER_VIEW lVbView = vertexBuffer->GetView();
	ID3D12GraphicsCommandList* lCmdList = sCmdList->GetGraphicCommandList();

	assert(camera_);

	if ( !material_ )
	{
		particleMaterial = MaterialManager::SGetMaterial("DefaultParticle");
	}

	AliceMathF::Matrix4 lMat = camera_->GetViewMatrix();
	lMat.m[ 3 ][ 0 ] = 0;
	lMat.m[ 3 ][ 1 ] = 0;
	lMat.m[ 3 ][ 2 ] = 0;
	lMat.m[ 3 ][ 3 ] = 1;

	constMapTransform.matBillboard = lMat;
	constMapTransform.mat = camera_->GetViewMatrixInv() * camera_->GetProjectionMatrix();
	constBuffTransform->Update(&constMapTransform);

	// パイプラインステートとルートシグネチャの設定コマンド
	lCmdList->SetPipelineState(particleMaterial->pipelineState->GetPipelineState());
	lCmdList->SetGraphicsRootSignature(particleMaterial->rootSignature->GetRootSignature());

	// プリミティブ形状の設定コマンド
	lCmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_POINTLIST); // 三角形リスト

	// 頂点バッファビューの設定コマンド
	lCmdList->IASetVertexBuffers(0,1,&lVbView);

	// 定数バッファビュー(CBV)の設定コマンド
	lCmdList->SetGraphicsRootConstantBufferView(0,constBuffTransform->GetAddress());

	// SRVヒープの設定コマンド
	ID3D12DescriptorHeap* lDescriptorHeaps[ ] = { textureData->srvHeap };
	lCmdList->SetDescriptorHeaps(1,lDescriptorHeaps);

	// SRVヒープの先頭にあるSRVをルートパラメータ1番に設定
	lCmdList->SetGraphicsRootDescriptorTable(1,textureData->gpuHandle);

	// 描画コマンド
	if ( std::distance(particleDatas.begin(),particleDatas.end()) < VERTEX_COUNT )
	{
		lCmdList->DrawInstanced(static_cast< UINT > ( std::distance(particleDatas.begin(),particleDatas.end()) ),1,0,0);
	}
	else
	{
		lCmdList->DrawInstanced(VERTEX_COUNT,1,0,0);
	}
}

void FireWorkParticle::Add(const AliceMathF::Vector3& pos_)
{
	//角度の最小単位
	float lAngleDiv = AliceMathF::AX_2PI / float(maxIndex);

	for ( size_t i = 0; i < maxIndex; i++ )
	{
		AliceMathF::Vector3 lVel{};
		//基準の速度で円形状に分布
		lVel.x = static_cast< float >( AliceMathF::Cos(lAngleDiv * static_cast< float >( i )) * ( particleVelocity.x / 2 ) );
		lVel.y = static_cast< float >( AliceMathF::Sin(lAngleDiv * static_cast< float >( i )) * ( particleVelocity.y / 2 ) );
		lVel.z = 0.0f;

		//基準の加速度でランダムに分布
		AliceMathF::Vector3 acc{};
		acc.y = static_cast< float >( rand() ) / RAND_MAX * particleAccel.y;

		PAdd(lifeTime,pos_,lVel,acc,particleScale,particleRotation,startColor,endColor);
	}
}

void FireWorkParticle::PAdd(uint32_t life_,const AliceMathF::Vector3& position_,const AliceMathF::Vector3& velocity_,const AliceMathF::Vector3& accel_,const AliceMathF::Vector2& scale_,const AliceMathF::Vector2& rotation_,const AliceMathF::Vector4& sColor_,const AliceMathF::Vector4& eColor_)
{
	if ( std::distance(particleDatas.begin(),particleDatas.end()) < VERTEX_COUNT )
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
