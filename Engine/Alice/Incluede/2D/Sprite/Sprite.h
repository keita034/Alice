#pragma once

#include<VertexBuffer.h>
#include<IndexBuffer.h>
#include<Transform.h>
#include<Camera.h>
#include<Material.h>
#include<WindowsApp.h>

enum BillboardFlag
{
	NonBillboard,//ビルボードをしない
	AllBillboard,//すべて打ち消す
	XBillboard,//X軸を打ち消す
	YBillboard,//Y軸を打ち消す
	XYBillboard//Z軸を打ち消す
};

class Sprite2D;
class Sprite3D;

class Sprite
{
protected:
	HRESULT result = S_OK;
	char PADDING[4]={};
	Microsoft::WRL::ComPtr<ID3D12Device> device;
	Microsoft::WRL::ComPtr <ID3D12GraphicsCommandList> cmdList = nullptr;

	//頂点バッファ
	std::unique_ptr<IVertexBuffer> vertexBuffer;

	//インデックスバッファ
	std::unique_ptr<IIndexBuffer> indexBuffer;

	//プロジェクション行列
	AliceMathF::Matrix4 matProjection;

	Material* spriteMaterial = nullptr ;

	bool InitializeFlag = true;
	char PADDING2[3]={};

	AliceMathF::Vector2 anchorPoint = { 0.5f, 0.5f };
	char PADDING3[4]={};

	TextureData* texture;

	//テクスチャ切り取り範囲{左上X,左上Y,右下X,右上Y}
	AliceMathF::Vector4 trimmingRange = { 0.0f,0.0f,1.0f,1.0f };
	// 左右反転
	bool flipX = false;
	// 上下反転
	bool flipY = false;
	char PADDING4[6]={};

	AliceMathF::Vector2 spriteSize;

	//カラー
	AliceMathF::Vector4 color = { 1.0f,1.0f,1.0f,1.0f };

	static IWindowsApp* windowsApp;

public:

	Sprite();
	virtual ~Sprite();

	//初期化
	virtual void Initialize(uint32_t handle) = 0;

	//描画
	virtual void Draw(Transform& transform, BlendMode blend, Material* material)=0;

	//アニメーション描画
	virtual void AnimationDraw(Transform& transform, uint16_t radiusX, uint16_t radiusY, float& frame, float frameDiv, BlendMode blend, Material* material) = 0;

	/// <summary>
	/// アンカーポイントの位置変更
	/// </summary>
	/// <param name="point">X,Y座標(0.0~1.0)初期値は0.5</param>
	virtual void SetAnchorPoint(const AliceMathF::Vector2& point);

	/// <summary>
	/// 画像を反転するか
	/// </summary>
	/// <param name="isFlipX">左右反転</param>
	/// <param name="isFlipY">上下反転</param>
	virtual void SetFlipFlag(bool isFlipX = false, bool isFlipY = false);

	/// <summary>
	/// カラーを設定
	/// </summary>
	/// <param name="color">RGBA</param>
	virtual void SetColor(const AliceMathF::Vector4& col);

	/// <summary>
	/// 画像の切り取り範囲を指定
	/// </summary>
	/// <param name="leftTop">左上座標 初期値は(0,0)</param>
	/// <param name="rightDown">右下座標 初期値は画像の大きさ</param>
	virtual void SetTextureTrimmingRange(const AliceMathF::Vector2& leftTop, const AliceMathF::Vector2& rightDown);

	/// <summary>
	/// テクスチャを設定
	/// </summary>
	/// <param name="handle">テクスチャハンドル</param>
	virtual void SetTex(uint32_t handle);

	virtual void SetSize(const AliceMathF::Vector2& size);

	/// <summary>
	/// 2Dスプライトを生成
	/// </summary>
	/// <param name="handle">テクスチャハンドル</param>
	static Sprite2D* Create2DSprite(uint32_t handle);

	/// <summary>
	/// 3Dスプライトを生成
	/// </summary>
	/// <param name="handle">テクスチャハンドル</param>
	static Sprite3D* Create3DSprite(uint32_t handle);

	/// <summary>
	/// テクスチャを取得
	/// </summary>
	const TextureData* GetTexture();

	static void SetWindowsApp(IWindowsApp* windowsApp_);

protected:

	//初期化
	virtual void SpriteInitialize();

	//描画
	virtual void SpriteDraw(Transform& transform, Material* material);


	//頂点バッファ・インデックス生成
	virtual void CreatVertexIndexBuffer();

	//コピーコンストラクタ・代入演算子削除
	Sprite& operator=(const Sprite&) = delete;
	Sprite(const Sprite&) = delete;
};