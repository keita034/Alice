#pragma once
//#DirectXのやつ
#include"ErrorException.h"

//自作.h
#include"AliceMathUtility.h"
#include"Transform.h"
#include"Mesh.h"
#include"Input.h"
#include"Sprite2D.h"

class Cursor
{
private:
	Input* input = nullptr;
	Mesh* mesh = nullptr;
	std::unique_ptr<Sprite2D> sprite;
	TextureData textureData;
	AliceMathF::Vector2 cursor;
public:

	void Update();
	void DrawCursor(float radiusX, float radiusY, AliceMathF::Vector4 color, bool fillMode);
	void DrawGraphCursor(Transform& transform);

	//ゲッター
	AliceMathF::Vector2 GetCursor();

	Cursor();
	Cursor(uint32_t handle);
	~Cursor();

private:

	// コピーコンストラクタを禁止（シングルトンパターン）
	Cursor(const Cursor& obj) = delete;
	// コピー代入演算子を禁止（シングルトンパターン）
	void operator=(const Cursor& obj) = delete;
};