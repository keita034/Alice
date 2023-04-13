#pragma once
#include"ErrorException.h"
#include"AliceMathUtility.h"
#include"Camera.h"
#include"ConstantBuffer.h"
#include"VertexBuffer.h"
#include"AliceUtility.h"
#include"Material.h"

struct VerPosColScaRot
{
	AliceMathF::Vector3 pos;//座標
	AliceMathF::Vector4 color;//カラー
	float scale;//スケール
	float rotation;//回転角
};

struct ParticleConstBuffData
{
	AliceMathF::Matrix4 mat;
	AliceMathF::Matrix4 matBillboard;
};

//パーティクルデータ
struct ParticleData
{
	//座標
	AliceMathF::Vector3 position = {};
	//速度
	AliceMathF::Vector3 velocity = {};
	//加速度
	AliceMathF::Vector3 accel = {};

	//現在フレーム
	UINT frame = 0;
	//終了フレーム
	UINT numFrame = 0;

	//スケール
	float scale = 1.0f;
	//初期値
	float sScale = 1.0f;
	//最終値
	float eScale = 0.0f;

	//回転角
	float rotation = 0.0f;
	//初期値
	float sRotation = 0.0f;
	//最終値
	float eRotation = 0.0f;

	//カラー
	AliceMathF::Vector4 color = {1.0f,1.0f,1.0f,1.0f};
	//初期値
	AliceMathF::Vector4 sColor = { 1.0f,1.0f,1.0f,1.0f };
	//最終値
	AliceMathF::Vector4 eColor = { 1.0f,1.0f,1.0f,1.0f };
};

class BasicParticle;

class RainParticle;

class Particle
{
protected:
	HRESULT result;
	char PADDING[4];
	Microsoft::WRL::ComPtr<ID3D12Device> device;
	Microsoft::WRL::ComPtr <ID3D12GraphicsCommandList> cmdList = nullptr;

	//頂点バッファ
	std::unique_ptr<VertexBuffer>vertexBuffer;
	//定数バッファ
	std::unique_ptr<ConstantBuffer> constBuffTransform = nullptr;
	//定数バッファのマッピング用
	ParticleConstBuffData constMapTransform{};

	//プロジェクション行列
	AliceMathF::Matrix4 matProjection;
	//頂点数
	const uint32_t vertexCount = 1024;
	char PADING[4];
	//パーティクル配列
	std::forward_list<ParticleData>particleDatas;

	Material* particleMaterial;

	TextureData* textureData;
public:

	Particle();

	virtual ~Particle();

	//初期化
	virtual void Initialize() = 0;

	///<summary>
	///更新
	///</summary>
	virtual void Update() = 0;

	///<summary>
	///描画
	///</summary>
	virtual void Draw(Camera* camera, Material* material) = 0;

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
		UINT life, const AliceMathF::Vector3& position, const AliceMathF::Vector3& velocity,
		const AliceMathF::Vector3& accel, const AliceMathF::Vector2& scale, const AliceMathF::Vector2& rotation
		, const AliceMathF::Vector4& sColor, const AliceMathF::Vector4& eColor) = 0;

	static BasicParticle* CreateParticle();

	static RainParticle* CreateRainParticle();

	void SetTex(uint32_t handle);

protected:

	//コピーコンストラクタ・代入演算子削除
	Particle& operator=(const Particle&) = delete;
	Particle(const Particle&) = delete;
};