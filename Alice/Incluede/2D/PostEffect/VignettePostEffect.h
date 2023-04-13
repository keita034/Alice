#pragma once
#include"BasePostEffect.h"
#include"ConstantBuffer.h"
#include"AliceMathUtility.h"

class VignettePostEffect : public BasePostEffect
{
private:

	std::unique_ptr<ConstantBuffer> vignetteDataBuff;

	struct VignetteData
	{
		//色
		AliceMathF::Vector3 color;
		float PADING1;
		//中心座標
		AliceMathF::Vector2 center;
		//強さ
		float power;
		float PADING2;
		//ウィンドウのサイズ(UV座標計算用)
		AliceMathF::Vector2 winSize;
		//円の縦横比
		AliceMathF::Vector2 size;
	};

	VignetteData data = {};

public:

	static VignettePostEffect* GetInstance();

	//初期化
	void Initialize() override;

	//更新
	void PostUpdate(RenderTarget* mainRenderTarget) override;

	/// <summary>
	/// ビネットの設定
	/// </summary>
	/// <param name="color">ビネットの色(255～0)</param>
	/// <param name="center">ビネットの中心座標(1.0～0.0)</param>
	/// <param name="power">ビネットの強さ</param>
	/// <param name="size">ビネットの縦横比(ウィンドウの比率に依存)</param>
	static void SetVignetteData(const AliceMathF::Vector3& color = { 1.0f,1.0f,1.0f }, const AliceMathF::Vector2& center = { 0.5f,0.5f }, float power = 1.0f, const AliceMathF::Vector2& size = { 1.0,1.0f });

	/// <summary>
	/// カラーを設定
	/// </summary>
	/// <param name="color">ビネットの色(255～0)</param>
	static void SetColor(const AliceMathF::Vector3& color = { 1.0f,1.0f,1.0f });

	/// <summary>
	/// 中心座標を設定
	/// </summary>
	/// <param name="center">ビネットの中心座標(1.0～0.0)</param>
	static void SetCenter(const AliceMathF::Vector2& center = { 0.5f,0.5f });

	/// <summary>
	/// 強さを設定
	/// </summary>
	/// <param name="power">ビネットの強さ</param>
	static void SetPower(float power = 1.0f);

	/// <summary>
	/// 縦横比を設定(ウィンドウの比率に依存)
	/// </summary>
	/// <param name="size">ビネットの縦横比</param>
	static void SetSize(const AliceMathF::Vector2& size = { 1.0,1.0f });

	/// <summary>
	/// タイプを取得
	/// </summary>
	const std::string& GetType()override;

private:

	void Draw(RenderTarget* mainRenderTarget)override;

	void MainRenderTargetDraw(RenderTarget* mainRenderTarget)override;

	//ビネットデータを設定
	void SetData(const AliceMathF::Vector3& color = { 1.0f,1.0f,1.0f }, const AliceMathF::Vector2& center = { 0.5f,0.5f }, float power = 1.0f, const AliceMathF::Vector2& size = { 1.0,1.0f });

	//ビネットカラーを設定
	void SetCol(const AliceMathF::Vector3& color = { 1.0f,1.0f,1.0f });

	//ビネットの中心座標を設定
	void SetCent(const AliceMathF::Vector2& center = { 0.5f,0.5f });

	//ビネットの強さを設定
	void SetPow(float power = 1.0f);

	//ビネットの縦横比を設定
	void SetSi(const AliceMathF::Vector2& size = { 1.0,1.0f });

	VignettePostEffect() = default;
	~VignettePostEffect() = default;

	//コピーコンストラクタ・代入演算子削除
	VignettePostEffect& operator=(const VignettePostEffect&) = delete;
	VignettePostEffect(const VignettePostEffect&) = delete;
};

