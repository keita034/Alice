#pragma once
#pragma warning(push)
#pragma warning(disable: 4365)
#pragma warning(disable: 4514)
#pragma warning(disable: 4619)
#pragma warning(disable: 4668)
#pragma warning(disable: 4711)
#pragma warning(disable: 4820)
#pragma warning(disable: 5039)

#include<memory>
#include<DirectXTex.h>

#pragma warning(pop)
#include<AliceMathUtility.h>

#include<DirectX12Core.h>
#include<TextureData.h>


class TextureManager
{
private:

	static DirectX12Core* sDirectX12Core;

	static std::unique_ptr<TextureManager> sTextureManager;

	//テクスチャ数
	uint32_t nextTexture;

	// ヒープ設定
	D3D12_HEAP_PROPERTIES textureHeapProp{};

	enum ImgFileType
	{
		WIC,//pngなど
		TGA,//tgaなど
		PSD,//psdなど
		ETC,//それ以外

	};

	static std::vector<std::string>sFilePaths;

	static std::unordered_map<std::string, std::unique_ptr<TextureData>> sTextureDatas;

public:

	/// <summary>
	/// テクスチャをロードします
	/// </summary>
	/// <param name="filepath">テクスチャのファイルパス</param>
	/// <returns>テクスチャハンドル</returns>
	uint32_t LoadTexture(const std::string& path_);

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// インスタンスを所得
	/// </summary>
	/// <returns>インスタンス</returns>
	static TextureManager* SGetInstance();

	/// <summary>
	/// テクスチャをロードします
	/// </summary>
	/// <param name="filepath">テクスチャのファイルパス</param>
	/// <returns>テクスチャハンドル</returns>
	static uint32_t SLoad(const std::string& path_);

	static TextureData* SGetTextureData(uint32_t handle_);

	static void SSetDirectX12Core(DirectX12Core* directX12Core_);

	void Finalize();

	~TextureManager() = default;

private:

	TextureManager() = default;


	Microsoft::WRL::ComPtr<ID3D12Resource>PCreateTexBuff(DirectX::TexMetadata& metadata_, DirectX::ScratchImage& scratchImg_);

	D3D12_GPU_DESCRIPTOR_HANDLE PCreateShaderResourceView(ID3D12Resource* texBuff_, const DirectX::TexMetadata& metadata_);

	void PLoadFile(const std::string& path_,  DirectX::TexMetadata& metadata_,  DirectX::ScratchImage& scratchImg_);

	std::unique_ptr<TextureData> PFromTextureData(const std::string& path_);

	ImgFileType PGetFileType(const std::string& path_);
};

