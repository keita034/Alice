﻿#pragma once
#include<AliceMathUtility.h>
#include<ConstantBuffer.h>

/// <summary>
/// ライト
/// </summary>

struct LightConstBuffData
{
	AliceMathF::Vector3 lightv;//ライトへの方向ベクトル
	float pad;
	AliceMathF::Vector4 lightcolor;//ライトの色
};

class Light
{
private:
	static ID3D12Device* sDevice;
	//定数バッファ
	std::unique_ptr<IConstantBuffer>constBuff;
	//定数バッファのマップ
	LightConstBuffData constMap;
	//ライト光線方向(単位ベクトル)
	AliceMathF::Vector3 lightdir = {1.0f,0.0f,0.0f};
	//ライトの色
	AliceMathF::Vector4 lightcolor = { 1,1,1,1 };
	//ダーティフラグ
	bool dirty = false;
	int8_t PADING[3]{};
public:

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 定数バッファへデータ転送
	/// </summary>
	void TransferConstBuffer();

	/// <summary>
	/// ライト方向のセット
	/// </summary>
	/// <param name="lightdir_">ライト方向</param>
	void SetLightDir(const AliceMathF::Vector3& lightdir_);

	/// <summary>
	/// ライト色のセット
	/// </summary>
	/// <param name="lightdir_">ライト方向</param>
	void SetLightColor(const AliceMathF::Vector4& lightcolor_);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 定数バッファビューのセット
	/// </summary>
	/// <param name="cmdlist">コマンドリスト</param>
	/// <param name="rootParameterIndex">パラメーター番号</param>
	void SetConstBufferView(ID3D12GraphicsCommandList* cmdList_,uint32_t rootParameterIndex_)const;

	static void SSetDevice(ID3D12Device* device_);

	Light() = default; 
	~Light() = default;
private:

	//コピーコンストラクタ・代入演算子削除
	Light& operator=(const Light&) = delete;
	Light(const Light&) = delete;

};

/// <summary>
/// ライトの生成(ユニーク)
/// </summary>
/// <returns>生成されたポインタ</returns>
std::unique_ptr<Light> CreateUniqueLight();

/// <summary>
/// ライトの生成(シェアード)
/// </summary>
/// <returns>生成されたポインタ</returns>
std::shared_ptr<Light> CreateSharedLight();
