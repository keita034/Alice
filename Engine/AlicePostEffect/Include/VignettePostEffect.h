#pragma once
#include<BasePostEffect.h>
#include<ConstantBuffer.h>
#include<AliceMathUtility.h>
#include<WindowsApp.h>

class VignettePostEffect : public BasePostEffect
{
private:

	std::unique_ptr<IConstantBuffer> vignetteDataBuff;

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

	static VignettePostEffect* SGetInstance();

	//初期化
	void Initialize() override;

	//更新
	void PostUpdate(RenderTarget* mainRenderTarget_) override;

	/// <summary>
	/// ビネットの設定
	/// </summary>
	/// <param name="color">ビネットの色(255～0)</param>
	/// <param name="center">ビネットの中心座標(1.0～0.0)</param>
	/// <param name="power">ビネットの強さ</param>
	/// <param name="size">ビネットの縦横比(ウィンドウの比率に依存)</param>
	static void SSetVignetteData(const AliceMathF::Vector3& color_ = { 1.0f,1.0f,1.0f }, const AliceMathF::Vector2& center_ = { 0.5f,0.5f }, float power_ = 1.0f, const AliceMathF::Vector2& size_ = { 1.0,1.0f });

	/// <summary>
	/// カラーを設定
	/// </summary>
	/// <param name="color">ビネットの色(255～0)</param>
	static void SSetColor(const AliceMathF::Vector3& color_ = { 1.0f,1.0f,1.0f });

	/// <summary>
	/// 中心座標を設定
	/// </summary>
	/// <param name="center">ビネットの中心座標(1.0～0.0)</param>
	static void SSetCenter(const AliceMathF::Vector2& center_ = { 0.5f,0.5f });

	/// <summary>
	/// 強さを設定
	/// </summary>
	/// <param name="power">ビネットの強さ</param>
	static void SSetPower(float power_ = 1.0f);

	/// <summary>
	/// 縦横比を設定(ウィンドウの比率に依存)
	/// </summary>
	/// <param name="size">ビネットの縦横比</param>
	static void SSetSize(const AliceMathF::Vector2& size_ = { 1.0,1.0f });

	/// <summary>
	/// タイプを取得
	/// </summary>
	const std::string& GetType()override;

private:

	void Draw(RenderTarget* mainRenderTarget_)override;

	void MainRenderTargetDraw(RenderTarget* mainRenderTarget_)override;

	//ビネットデータを設定
	void PSetVignetteData(const AliceMathF::Vector3& color_ = { 1.0f,1.0f,1.0f }, const AliceMathF::Vector2& center_ = { 0.5f,0.5f }, float power_ = 1.0f, const AliceMathF::Vector2& size_ = { 1.0,1.0f });

	//ビネットカラーを設定
	void PSetColor(const AliceMathF::Vector3& color_ = { 1.0f,1.0f,1.0f });

	//ビネットの中心座標を設定
	void PSetCenter(const AliceMathF::Vector2& center_ = { 0.5f,0.5f });

	//ビネットの強さを設定
	void PSetPower(float power_ = 1.0f);

	//ビネットの縦横比を設定
	void PSetSize(const AliceMathF::Vector2& size_ = { 1.0,1.0f });

	VignettePostEffect() = default;
	~VignettePostEffect() = default;

private:

	//コピーコンストラクタ・代入演算子削除
	VignettePostEffect& operator=(const VignettePostEffect&) = delete;
	VignettePostEffect(const VignettePostEffect&) = delete;
};

