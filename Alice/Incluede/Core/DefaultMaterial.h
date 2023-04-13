#pragma once
#include"Material.h"
#include"TextureManager.h"
#include"AliceMathUtility.h"

class MaterialManager
{
private:

	TextureData* DEFAULT_TEXTURE;

	std::unordered_map<std::string, std::unique_ptr<Material>>materials;
public:

	void Initialize();

	static MaterialManager* GetInstance();

	Material* GetMaterialData(const std::string name);

	void AddMaterial(std::unique_ptr <Material>& material, const std::string name);
	void AddMaterial(Material* material, const std::string name);

	static Material* GetMaterial(const std::string name);

	TextureData* GetDefaultTextureData();

	static TextureData* GetDefaultTexture();

private:

	MaterialManager() = default;
	~MaterialManager();

	void CreateDefaultTexture();

	void CreateDefaultTextureMaterial();

	void CreateDefaultLambertMaterial();

	void CreateDefaultPhongMaterial();

	void CreateDefaultSprite2DMaterial();

	void CreateDefaultSprite3DMaterial();

	void CreateDefaultFbxMaterial();

	void CreateDefaultFbxAnimationMaterial();

	void CreateDefaultParticleMaterial();

	void CreateDefaultMeshMaterial();

	void CreateDefaultRainParticleMaterial();

	void CreateDefaultPostEffectMaterial();

	void CreateDefaultIcosahedronParticleMaterial();

	Material* CreateDefaultMeshBlend(D3D12_PRIMITIVE_TOPOLOGY_TYPE type, BlendMode mode, Shader* vex, Shader* pix);

	Material* CreateDefaultSprite2DBlend(BlendMode mode, Shader* vex, Shader* pix);

	Material* CreateDefaultSprite3DBlend(BlendMode mode, Shader* vex, Shader* pix);

	D3D12_BLEND_DESC CreateBlend(BlendMode mode);

   //Rs[RXgN^EăüZqí
	MaterialManager& operator=(const MaterialManager&) = delete;
	MaterialManager(const MaterialManager&) = delete;
};
