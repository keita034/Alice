#pragma once
#include "BaseCollider.h"
#include"TriangleCollider.h"
#include"Structure.h"

class MeshCollider : public BaseCollider
{
private:

	//メッシュ配列
	std::vector<TriangleCollider> triangles;

	//メッシュの数
	size_t trianglesNum;

public:

	/// <summary>
	/// メッシュをセット
	/// </summary>
	/// <param name="modelMeshs">メッシュ</param>
	/// <param name="indexs">インデックス配列</param>
	void SetMeshs(const std::vector<PosColor>& modelMeshs, const std::vector<uint32_t>& indexs);

	/// <summary>
	/// メッシュをセット
	/// </summary>
	/// <param name="modelMeshs">メッシュ</param>
	/// <param name="indexs">インデックス配列</param>
	void SetMeshs(const std::vector<PosNorm>& modelMeshs, const std::vector<uint32_t>& indexs);

	/// <summary>
	/// メッシュをセット
	/// </summary>
	/// <param name="modelMeshs">メッシュ</param>
	/// <param name="indexs">インデックス配列</param>
	void SetMeshs(const std::vector<PosNormalUv>& modelMeshs, const std::vector<uint32_t>& indexs);

	/// <summary>
	/// メッシュをセット
	/// </summary>
	/// <param name="modelMeshs">メッシュ</param>
	/// <param name="indexs">インデックス配列</param>
	void SetMeshs(const std::vector<PosNormSkin>& modelMeshs, const std::vector<uint32_t>& indexs);

	/// <summary>
	/// メッシュをセット
	/// </summary>
	/// <param name="modelMeshs">メッシュ</param>
	/// <param name="indexs">インデックス配列</param>
	void SetMeshs(const std::vector<PosNormUvTangeColSkin>& modelMeshs, const std::vector<uint32_t>& indexs);

	/// <summary>
	/// 三角形配列を取得
	/// </summary>
	const std::vector<TriangleCollider>& GetTriangles();

	/// <summary>
	/// 三角形の数を取得
	/// </summary>
	size_t GetTrianglesNum();

	/// <summary>
	/// 三角形を取得
	/// </summary>
	/// <param name="index">要素番号</param>
	const TriangleCollider& GetTriangles(size_t index);

	MeshCollider() = default;
	virtual ~MeshCollider() = default;

private:
};

