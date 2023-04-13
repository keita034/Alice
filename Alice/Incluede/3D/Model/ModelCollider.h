#pragma once
#include "BaseCollider.h"
#include "MeshCollider.h"

class ModelMesh;

class ModelCollider :public BaseCollider
{
private:

	//メッシュ
	std::vector<MeshCollider> meshs;

	//メッシュの数
	size_t meshsNum;

public:

	ModelCollider() = default;

	virtual ~ModelCollider() = default;

	/// <summary>
	/// メッシュをセット
	/// </summary>
	/// <param name="modelMeshs">メッシュ</param>
	void SetMeshs(std::vector<ModelMesh> modelMeshs);

	/// <summary>
	/// メッシュ配列を取得
	/// </summary>
	const std::vector<MeshCollider>& GetMeshs();

	/// <summary>
	/// メッシュの数を取得
	/// </summary>
	size_t GetMeshNum();

	/// <summary>
	/// メッシュを取得
	/// </summary>
	/// <param name="index">要素番号</param>
	const MeshCollider& GetMeshs(size_t index);

private:

	//コピーコンストラクタ・代入演算子削除
	ModelCollider& operator=(const ModelCollider&) = delete;
	ModelCollider(const ModelCollider&) = delete;

};