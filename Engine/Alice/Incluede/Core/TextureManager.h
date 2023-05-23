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

#include<AliceFunctionUtility.h>
#include<DirectX12Core.h>

struct TextureData
{
	//テクスチャバッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> texBuff;

	//デスクプリタヒープ
	ID3D12DescriptorHeap* srvHeap;

	//GPUデスクプリタハンドル
	D3D12_GPU_DESCRIPTOR_HANDLE gpuHandle{};

	//横幅
	size_t width = 0;
	//縦幅
	size_t height = 0;

	//カラー
	AliceMathF::Vector4 color = { 1.0f,1.0f,1.0f,1.0f };

	std::string path;

	uint32_t textureHandle;

	~TextureData();

private:
	char PADING[4]{};
};

class TextureManager
{
private:

	DirectX12Core* directX12Core;

	static TextureManager* textureManager;

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

	static std::vector<std::string>filePaths;

	static std::unordered_map<std::string, std::unique_ptr<TextureData>> textureDatas;

public:

	/// <summary>
	/// テクスチャをロードします
	/// </summary>
	/// <param name="filepath">テクスチャのファイルパス</param>
	/// <returns>テクスチャハンドル</returns>
	uint32_t LoadTexture(const std::string& path);

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// インスタンスを所得
	/// </summary>
	/// <returns>インスタンス</returns>
	static TextureManager* GetInstance();

	/// <summary>
	/// インスタンスを解放
	/// </summary>
	void Destroy();

	/// <summary>
	/// テクスチャをロードします
	/// </summary>
	/// <param name="filepath">テクスチャのファイルパス</param>
	/// <returns>テクスチャハンドル</returns>
	static uint32_t Load(const std::string& path);

	static TextureData* GetTextureData(uint32_t handle);

private:

	TextureManager() = default;
	~TextureManager() = default;

	Microsoft::WRL::ComPtr<ID3D12Resource>CreateTexBuff(DirectX::TexMetadata& metadata, DirectX::ScratchImage& scratchImg);

	D3D12_GPU_DESCRIPTOR_HANDLE CreateShaderResourceView(ID3D12Resource* texBuff, DirectX::TexMetadata& metadata);

	void LoadFile(const std::string& path, DirectX::TexMetadata& metadata, DirectX::ScratchImage& scratchImg);

	TextureData* FromTextureData(const std::string& path);

	ImgFileType GetFileType(const std::string& path);
};

