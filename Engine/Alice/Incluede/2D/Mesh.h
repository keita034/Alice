#pragma once

//自作.h

#include<WindowsApp.h>
#include<AliceMathUtility.h>
#include<ConstantBuffer.h>
#include<AliceUtility.h>
#include<DefaultMaterial.h>

//using namespace

class Mesh
{
private:

	static ID3D12GraphicsCommandList* sCmdList;
	static IWindowsApp* sWindowsApp;

	//定数バッファのGPUリソースのポインタ
	std::unique_ptr<IConstantBuffer> constBuffTransform;
	//定数バッファのマッピング用
	ConstBufferDataTransform constMapTransform;

	//ブレンドモード
	uint32_t blendMode;
	char PADDING[4];

	//三角形バッファ
	std::unique_ptr <Buff> triangleBuff;
	//三角形描画上限
	uint32_t triangleMaxCount = 4096;
	//三角形頂点数
	uint32_t triangleVertexCount = 3;
	//三角形インデックス数
	uint32_t triangleIndexCount = 0;
	//三角形使用数
	uint32_t triangleCount = 0;

	//線バッファ
	std::unique_ptr <Buff> lineBuff;
	//線描画上限
	uint32_t lineMaxCount = 4096;
	//線頂点数
	uint32_t lineVertexCount = 2;
	//線インデックス数
	uint32_t lineIndexCount = 0;
	//線使用数
	uint32_t lineCount = 0;

	//四角形バッファ
	std::unique_ptr <Buff> boxBuff;
	//四角形描画上限
	uint32_t boxMaxCount = 2048;
	//四角形頂点数
	uint32_t boxVertexCount = 4;
	//四角形インデックス数
	uint32_t boxIndexCount = 6;
	//四角形使用数
	uint32_t boxCount = 0;

	static std::unique_ptr<Mesh> mesh;

public:

	///<summary>
	///三角形を描画する
	///</summary>
	///<param name="x1_">: 描画する三角形の頂点1 X</param>
	///<param name="y1_">: 描画する三角形の頂点1 Y</param>
	///<param name="x2_">: 描画する三角形の頂点2 X</param>
	///<param name="y2_">: 描画する三角形の頂点2 Y</param>
	///<param name="x3_">: 描画する三角形の頂点3 X</param>
	///<param name="y3_">: 描画する三角形の頂点3 Y</param>
	///<param name="color_">: 描画する三角形の色</param>
	///<param name="fillFlag_">: 三角形の中身を塗りつぶすかフラグ</param>
	void DrawTriangle(float x1_, float y1_, float x2_, float y2_, float x3_, float y3_, const AliceMathF::Vector4& color_, int32_t fillFlag_);

	///<summary>
	///線を描画する
	///</summary>
	///<param name="x1_">: 描画する線の始点座標 X</param>
	///<param name="y1_">: 描画する線の始点座標 Y</param>
	///<param name="x2_">: 描画する線の終点座標 X</param>
	///<param name="y2_">: 描画する線の終点座標 Y</param>
	///<param name="color_">: 描画する線の色</param>
	void DrawLine(float x1_, float y1_, float x2_, float y2_, const AliceMathF::Vector4& color_);

	///<summary>
	///四角形を描画する
	///</summary>
	///<param name="x">: 描画する四角形の中心座標 X</param>
	///<param name="y">: 描画する四角形の中心座標 Y</param>
	///<param name="radiusX">: 描画する四角形の半径 X</param>
	///<param name="radiusY">: 描画する四角形の半径 Y</param>
	///<param name="angle">: 描画する四角形の回転角(弧度法)</param>
	///<param name="color_">: 描画する四角形の色</param>
	///<param name="fillFlag_">: 四角形の中身を塗りつぶすかフラグ</param>
	void DrawBox(float x_, float y_, float radiusX_, float radiusY_, float angle_, const AliceMathF::Vector4& color_, int32_t fillFlag_);

	///<summary>
	///楕円を描画する
	///</summary>
	///<param name="x">楕円の中心座標 X</param>
	///<param name="y">楕円の中心座標 Y</param>
	///<param name="radiusX">楕円の半径 X</param>
	///<param name="radiusY">楕円の半径 Y</param>
	///<param name="angle">楕円の回転角(rad)</param>
	///<param name="color_">楕円の色</param>
	///<param name="fillFlag_">: 四角形の中身を塗りつぶすかフラグ</param>
	void DrawEllipse(float x_, float y_, float radiusX_, float radiusY_, float angle_, const AliceMathF::Vector4& color_, int32_t fillMode_);

	///<summary>
	///カウント初期化
	///</summary>
	void DrawReset();

	///<summary>
	///ブレンドモードを設定する
	///</summary>
	///<param name="mode">: ブレンドモード</param>
	void SetBlendMode(BlendMode mode_);

	///<summary>
	///色コードを取得する
	///</summary>
	///<param name="red">: 取得したい色の輝度値 : 初期値255 (0～255)</param>
	///<param name="blue">: 取得したい色の輝度値 : 初期値255 (0～255)</param>
	///<param name="green">: 取得したい色の輝度値 : 初期値255 (0～255)</param>
	///<param name="alpha">: 取得したい色の透過率 : 初期値255 (0～255)</param>
	///<returns>色コード</returns>
	AliceMathF::Vector4 GetColor(int32_t red_ = 255, int32_t blue_ = 255, int32_t green_ = 255, int32_t alpha_ = 255);

	///<summary>
	///色コードを取得する
	///</summary>
	///<param name="color_">: 取得したい各色の輝度値 : 初期値255 (0～255)</param>
	///<returns>色コード</returns>
	AliceMathF::Vector4 GetColor(const AliceMathF::Vector4& color_);

	/// <summary>
	/// インスタンスを所得
	/// </summary>
	/// <returns>インスタンス</returns>
	static Mesh* GetInstance();

	/// <summary>
	/// インスタンスを解放
	/// </summary>
	void Destroy();

	static void SSetWindowsApp(IWindowsApp* windowsApp_);

	static void SSetDirectX12Core(DirectX12Core* directX12Core_);

private:

	//コンストラクタ
	Mesh();

	//三角形を描画する(中身塗りつぶし)
	void PDrawTriangleFill(float x1_, float y1_, float x2_, float y2_, float x3_, float y3_, const AliceMathF::Vector4& color_);

	//四角形を描画する(中身塗りつぶし)
	void PDrawBoxFill(float x_, float y_, float radiusX_, float radiusY_, float angle_, AliceMathF::Vector4 color_);

	///<summary>
	///バッファ作成
	///</summary>
	///<param name="vertexCount">: 頂点数</param>
	///<param name="indexCount">: インデックス数</param>
	///<returns>バッファ</returns>
	std::unique_ptr <Buff> PCreateBuff(uint32_t vertexCount_, uint32_t indexCount_);

	//定数バッファ生成(2D座標変換行列)
	void PCreatConstBuff();

	//コピーコンストラクタ・代入演算子削除
	Mesh& operator=(const Mesh&) = delete;
	Mesh(const Mesh&) = delete;
};



