#pragma once

#include<VertexBuffer.h>
#include<IndexBuffer.h>
#include<Transform.h>
#include<Camera.h>
#include<Material.h>
#include<WindowsApp.h>

enum BillboardFlag
{
	NON_BILLBOARD,//ビルボードをしない
	ALL_BILLBOARD,//すべて打ち消す
	X_BILLBOARD,//X軸を打ち消す
	Y_BILLBOARD,//Y軸を打ち消す
	XY_BILLBOARD//Z軸を打ち消す
};

class Sprite
{
protected:
	static IWindowsApp* sWindowsApp;
	static ID3D12Device* sDevice;
	static ID3D12GraphicsCommandList* sCmdList;

	//頂点バッファ
	std::unique_ptr<IVertexBuffer> vertexBuffer;

	//インデックスバッファ
	std::unique_ptr<IIndexBuffer> indexBuffer;

	//プロジェクション行列
	AliceMathF::Matrix4 matProjection;

	const Material* spriteMaterial = nullptr ;

	AliceMathF::Vector2 anchorPoint = { 0.5f, 0.5f };

	TextureData* texture;

	//テクスチャ切り取り範囲{左上X,左上Y,右下X,右上Y}
	AliceMathF::Vector4 trimmingRange = { 0.0f,0.0f,1.0f,1.0f };
	// 左右反転
	bool flipX = false;
	// 上下反転
	bool flipY = false;

	bool InitializeFlag = true;

	int8_t PADING = 0;

	HRESULT result = S_OK;

	AliceMathF::Vector2 spriteSize;

	//カラー
	AliceMathF::Vector4 color = { 1.0f,1.0f,1.0f,1.0f };

public:

	Sprite();
	virtual ~Sprite();

	//初期化
	virtual void Initialize(uint32_t handle_) = 0;

	//描画
	virtual void Draw(Transform& transform_, BlendMode blend_, const Material* material_)=0;

	//アニメーション描画
	virtual void AnimationDraw(Transform& transform_, uint16_t radiusX_, uint16_t radiusY_, float& frame_, float frameDiv_, BlendMode blend_, const Material* material_) = 0;

	/// <summary>
	/// アンカーポイントの位置変更
	/// </summary>
	/// <param name="point">X,Y座標(0.0~1.0)初期値は0.5</param>
	virtual void SetAnchorPoint(const AliceMathF::Vector2& point_);

	/// <summary>
	/// 画像を反転するか
	/// </summary>
	/// <param name="isFlipX">左右反転</param>
	/// <param name="isFlipY">上下反転</param>
	virtual void SetFlipFlag(bool isFlipX_ = false, bool isFlipY_ = false);

	/// <summary>
	/// カラーを設定
	/// </summary>
	/// <param name="color">RGBA</param>
	virtual void SetColor(const AliceMathF::Vector4& color_);

	/// <summary>
	/// 画像の切り取り範囲を指定
	/// </summary>
	/// <param name="leftTop">左上座標 初期値は(0,0)</param>
	/// <param name="rightDown">右下座標 初期値は画像の大きさ</param>
	virtual void SetTextureTrimmingRange(const AliceMathF::Vector2& leftTop_, const AliceMathF::Vector2& rightDown_);

	/// <summary>
	/// テクスチャを設定
	/// </summary>
	/// <param name="handle">テクスチャハンドル</param>
	virtual void SetTex(uint32_t handle_);

	virtual void SetSize(const AliceMathF::Vector2& size_);

	/// <summary>
	/// テクスチャを取得
	/// </summary>
	const TextureData* GetTexture();

	static void SSetWindowsApp(IWindowsApp* windowsApp_);

	static void SSetDirectX12Core(DirectX12Core* directX12Core_);

protected:

	//描画
	virtual void PSpriteDraw(const Transform& transform_, const Material* material_);

	//頂点バッファ・インデックス生成
	virtual void PCreatVertexIndexBuffer();

	//コピーコンストラクタ・代入演算子削除
	Sprite& operator=(const Sprite&) = delete;
	Sprite(const Sprite&) = delete;
};