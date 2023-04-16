#pragma once

#include<AliceMathUtility.h>
#include<Transform.h>
#include<Mesh.h>
#include<Input.h>
#include<Sprite2D.h>

class Button
{
private:
	Input* input = nullptr;
	Mesh* mesh = nullptr;
	std::unique_ptr<Sprite2D> sprite;
	TextureData textureData;

public:
	Button();
	Button(uint32_t handle);
	~Button();

	/// <summary>
	/// ボタン生成
	/// </summary>
	///<param name="x">: 描画するボタンの中心座標 X</param>
	///<param name="y">: 描画するボタンの中心座標 Y</param>
	///<param name="radiusX">: 描画するボタンの横の半径 radiusX</param>
	///<param name="radiusY">: 描画するボタンの縦の半径 radiusY</param>
	///<param name="color">: 描画する線の色</param>
	///<param name="fillFlag">: ボタンの中身を塗りつぶすかフラグ</param>
	///<param name="posX">: カーソル座標 X</param>
	///<param name="posY">: カーソル座標 Y</param>
	///<param name="sceneFlag">: ボタンを押した時に更新したい変数名</param>
	///<param name="sceneNextNum">: ボタンを押した後にsceneFlagにセットしたい値</param>
	void DrawButton(float x,float y, float radiusX, float radiusY, AliceMathF::Vector4 color,float posX,float posY, bool fillFlag,int& sceneFlag,int sceneNextNum);

	/// <summary>
	/// リソース版ボタン生成
	/// </summary>
	///<param name="textureData">: 描画するボタンのテクスチャ textureData</param>
	///<param name="transform">: 描画するボタンの中心座標 Y</param>
	///<param name="posX">: カーソル座標 X</param>
	///<param name="posY">: カーソル座標 Y</param>
	///<param name="sceneFlag">: ボタンを押した時に更新したい変数名</param>
	///<param name="sceneNextNum">: ボタンを押した後にsceneFlagにセットしたい値</param>
	void DrawGraphButton(Transform& transform, float posX, float posY, int& sceneFlag, int sceneNextNum);

private:
	// コピーコンストラクタを禁止（シングルトンパターン）
	Button(const Button& obj) = delete;
	// コピー代入演算子を禁止（シングルトンパターン）
	void operator=(const Button& obj) = delete;
};