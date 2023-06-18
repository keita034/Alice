#pragma once
#include<Material.h>
#include<TextureManager.h>

class MaterialManager
{
private:

	TextureData* defaultTexture;

	std::unordered_map<std::string, std::unique_ptr<Material>>materials;
public:

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(ID3D12Device* device_);

	static MaterialManager* SGetInstance();

	/// <summary>
	/// マテリアル取得
	/// </summary>
	/// <param name="name_">名前</param>
	Material* GetMaterialData(const std::string& name_);

	/// <summary>
	/// マテリアル追加
	/// </summary>
	void AddMaterial(std::unique_ptr <Material>& material_, const std::string& name_);

	/// <summary>
	/// マテリアル追加
	/// </summary>
	void AddMaterial(Material* material_, const std::string& name_);

	/// <summary>
	/// マテリアル取得
	/// </summary>
	/// <param name="name_">名前</param>
	static Material* SGetMaterial(const std::string& name_);

	TextureData* GetDefaultTextureData();

	static TextureData* SGetDefaultTexture();

	static ID3D12Device* sDevice;

private:

	MaterialManager() = default;
	~MaterialManager();

	void PCreateDefaultTexture();
	void PCreateDefaultTextureMaterial();
	void PCreateDefaultLambertMaterial();
	void PCreateDefaultPhongMaterial();
	void PCreateDefaultSprite2DMaterial();
	void PCreateDefaultSprite3DMaterial();
	void PCreateDefaultFbxMaterial();
	void PCreateDefaultFbxAnimationMaterial();
	void PCreateDefaultParticleMaterial();
	void PCreateDefaultMeshMaterial();
	void PCreateDefaultRainParticleMaterial();
	void PCreateDefaultPostEffectMaterial();
	void PCreateDefaultIcosahedronParticleMaterial();
	void PCreateDefaultToonModelMaterial();
	void PCreateDefaultToonModelAnimationMaterial();
	void PCreateDefaultToonModelOutLineMaterial();
	void PCreateDefaultToonModelOutLineAnimationMaterial();
	void PCreateDefaultZeldaToonModelMaterial();
	void PCreateDefaultZeldaToonModelAnimationMaterial();

	std::unique_ptr<Material> PCreateDefaultMeshBlend(D3D12_PRIMITIVE_TOPOLOGY_TYPE type_, BlendMode mode_, IShader* vex_, IShader* pix_);
	std::unique_ptr<Material> PCreateDefaultSprite2DBlend(BlendMode mode_, IShader* vex_, IShader* pix_);
	std::unique_ptr<Material> PCreateDefaultSprite3DBlend(BlendMode mode_, IShader* vex_, IShader* pix_);

	D3D12_BLEND_DESC PCreateBlend(BlendMode mode_);

	//コピーコンストラクタ・代入演算子削除
	MaterialManager& operator=(const MaterialManager&) = delete;
	MaterialManager(const MaterialManager&) = delete;
};
