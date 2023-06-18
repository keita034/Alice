#pragma once
#pragma warning(push)
#pragma warning(disable: 4365)
#pragma warning(disable: 4514)
#pragma warning(disable: 4619)
#pragma warning(disable: 4668)
#pragma warning(disable: 5039)

#include<forward_list>

#pragma warning(pop)

#include<VertexBuffer.h>
#include<AliceUtility.h>
#include<Material.h>
#include<ConstantBuffer.h>
#include<Camera.h>
#include<TextureManager.h>

#include<ParticleData.h>

class Particle
{
protected:
	static ID3D12Device* sDevice;
	static ID3D12GraphicsCommandList* sCmdList;
	static TextureManager* sTextureManager;

	//頂点バッファ
	std::unique_ptr<IVertexBuffer>vertexBuffer;
	//定数バッファ
	std::unique_ptr<IConstantBuffer> constBuffTransform;
	//定数バッファのマッピング用
	ParticleConstBuffData constMapTransform{};

	//プロジェクション行列
	AliceMathF::Matrix4 matProjection;
	//頂点数
	const uint32_t VERTEX_COUNT = 1024;

	HRESULT result;

	//パーティクル配列
	std::forward_list<ParticleData>particleDatas;

	Material* particleMaterial;

	TextureData* textureData;

public:

	Particle();

	virtual ~Particle();

	void SetTex(uint32_t handle_);

	static void SSetTextureManager(TextureManager* textureManager_);

	static void SSetDirectX12Core(DirectX12Core* directX12Core_);

protected:

	//コピーコンストラクタ・代入演算子削除
	Particle& operator=(const Particle&) = delete;
	Particle(const Particle&) = delete;
};