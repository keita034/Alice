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
	void SetMeshs(const std::vector<PosColor>& modelMeshs_, const std::vector<uint32_t>& indexs_);

	/// <summary>
	/// メッシュをセット
	/// </summary>
	/// <param name="modelMeshs_">メッシュ</param>
	/// <param name="indexs">インデックス配列</param>
	void SetMeshs(const std::vector<PosNorm>& modelMeshs_, const std::vector<uint32_t>& indexs_);

	/// <summary>
	/// メッシュをセット
	/// </summary>
	/// <param name="modelMeshs_">メッシュ</param>
	/// <param name="indexs">インデックス配列</param>
	void SetMeshs(const std::vector<PosNormalUv>& modelMeshs_, const std::vector<uint32_t>& indexs_);

	/// <summary>
	/// メッシュをセット
	/// </summary>
	/// <param name="modelMeshs_">メッシュ</param>
	/// <param name="indexs">インデックス配列</param>
	void SetMeshs(const std::vector<PosNormSkin>& modelMeshs_, const std::vector<uint32_t>& indexs_);

	/// <summary>
	/// メッシュをセット
	/// </summary>
	/// <param name="modelMeshs_">メッシュ</param>
	/// <param name="indexs">インデックス配列</param>
	void SetMeshs(const std::vector<PosNormUvTangeColSkin>& modelMeshs_, const std::vector<uint32_t>& indexs_);

	/// <summary>
	/// 三角形配列を取得
	/// </summary>
	const std::vector<TriangleCollider>& GetTriangles()const;

	/// <summary>
	/// 三角形の数を取得
	/// </summary>
	size_t GetTrianglesNum();

	/// <summary>
	/// 三角形を取得
	/// </summary>
	/// <param name="index">要素番号</param>
	const TriangleCollider& GetTriangles(size_t index_)const;

	MeshCollider() = default;
	virtual ~MeshCollider() = default;

	MeshCollider(const MeshCollider&) = default;

};

