#pragma once
#include<Defined.h>

ALICE_SUPPRESS_WARNINGS_BEGIN

#include<unordered_map>

ALICE_SUPPRESS_WARNINGS_END

#include<BlendMode.h>
#include<Material.h>
#include<ComputeMaterial.h>
#include<MultiAdapters.h>

class MaterialManager
{
private:
	static std::unique_ptr<MaterialManager>materialManager;

	std::unordered_map<std::string,std::unique_ptr<Material>>materials;
	std::unordered_map<std::string, std::unique_ptr<ComputeMaterial>>computeMaterials;
	IMultiAdapters* sMultiAdapters;

public:

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(IMultiAdapters* multiAdapters_);

	static MaterialManager* SGetInstance();

	/// <summary>
	/// マテリアル取得
	/// </summary>
	/// <param name="name_">名前</param>
	Material* GetMaterialData(const std::string& name_,AdaptersIndex index_ = AdaptersIndex::MAIN);

	ComputeMaterial* GetComputeMaterialData(const std::string& name_,AdaptersIndex index_ = AdaptersIndex::SUB);

	/// <summary>
	/// マテリアル追加
	/// </summary>
	void AddMaterial(std::unique_ptr <Material>& material_,const std::string& name_);

	void AddMaterial(std::unique_ptr <ComputeMaterial>& material_, const std::string& name_);

	/// <summary>
	/// マテリアル追加
	/// </summary>
	void AddMaterial(Material* material_, const std::string& name_);

	void Finalize();

	/// <summary>
	/// マテリアル取得
	/// </summary>
	/// <param name="name_">名前</param>
	static Material* SGetMaterial(const std::string& name_,AdaptersIndex index_ = AdaptersIndex::MAIN);

	/// <summary>
	/// マテリアル取得
	/// </summary>
	/// <param name="name_">名前</param>
	static ComputeMaterial* SGetComputeMaterial(const std::string& name_,AdaptersIndex index_ = AdaptersIndex::SUB);

	MaterialManager() = default;
	~MaterialManager();

private:

	//コピーコンストラクタ・代入演算子削除
	MaterialManager& operator=(const MaterialManager&) = delete;
	MaterialManager(const MaterialManager&) = delete;
};
