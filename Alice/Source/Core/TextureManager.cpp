#pragma warning(push)
#pragma warning(disable: 4062)
#pragma warning(disable: 4365)
#pragma warning(disable: 4514)
#pragma warning(disable: 4619)
#pragma warning(disable: 4668)
#pragma warning(disable: 4711)
#pragma warning(disable: 4820)
#pragma warning(disable: 5039)

#include<directx/d3dx12.h>

#pragma warning(pop)

#include "TextureManager.h"
#include"AliceFunctionUtility.h"

TextureManager* TextureManager::textureManager = nullptr;
std::vector<std::string>TextureManager::filePaths;
std::unordered_map<std::string, std::unique_ptr<TextureData>> TextureManager::textureDatas;

void TextureManager::LoadFile(const std::string& path, DirectX::TexMetadata& metadata, DirectX::ScratchImage& scratchImg)
{
	wchar_t wfilepath[256];
	HRESULT result = 0;

	switch (GetFileType(path))
	{
	case WIC:
		MultiByteToWideChar(CP_ACP, 0, path.c_str(), -1, wfilepath, _countof(wfilepath));
		// WICテクスチャのロード
		result = LoadFromWICFile(
			wfilepath,
			DirectX::WIC_FLAGS_NONE,
			&metadata, scratchImg);
		assert(SUCCEEDED(result));
		break;

	case TGA:
		MultiByteToWideChar(CP_ACP, 0, path.c_str(), -1, wfilepath, _countof(wfilepath));
		// TGAテクスチャのロード
		result = LoadFromTGAFile(
			wfilepath,
			&metadata, scratchImg);
		assert(SUCCEEDED(result));
		break;

	case PSD:
	{
		std::string texPath = AliceFunctionUtility::ReplaceExtension(path, "tga");

		MultiByteToWideChar(CP_ACP, 0, texPath.c_str(), -1, wfilepath, _countof(wfilepath));

		// TGAテクスチャのロード
		result = LoadFromTGAFile(
			wfilepath,
			&metadata, scratchImg);
		assert(SUCCEEDED(result));
		break;
	}
	case ETC:
		assert(0);
		break;
	default:
		assert(0);
		break;
	}
}

TextureData* TextureManager::FromTextureData(const std::string& path)
{
	TextureData* result = new TextureData();

	DirectX::TexMetadata metadata{};
	DirectX::ScratchImage scratchImg{};
	DirectX::ScratchImage mipChain{};

	result->srvHeap = directX12Core->GetSRVDescriptorHeap()->GetHeap();

	LoadFile(path, metadata, scratchImg);

	//ミップマップ生成
	HRESULT hr = GenerateMipMaps(
		scratchImg.GetImages(), scratchImg.GetImageCount(), scratchImg.GetMetadata(),
		DirectX::TEX_FILTER_DEFAULT, 0, mipChain);

	if (SUCCEEDED(hr))
	{
		scratchImg = std::move(mipChain);
		metadata = scratchImg.GetMetadata();
	}

	//読み込んだディフューズテクスチャをSRGBとして扱う
	metadata.format = DirectX::MakeSRGB(metadata.format);

	result->texBuff = CreateTexBuff(metadata, scratchImg);

	result->gpuHandle = CreateShaderResourceView(result->texBuff.Get(), metadata);

	result->width = metadata.width;
	result->height = metadata.height;

	return result;
}

TextureManager::ImgFileType TextureManager::GetFileType(const std::string& path)
{
	std::string extend = AliceFunctionUtility::FileExtension(path);
	if (extend == "png" ||
		extend == "bmp" ||
		extend == "gif" ||
		extend == "tiff"||
		extend == "jpeg"||
		extend == "jpg")
	{
		return WIC;
	}
	else if (extend == "tga")
	{
		return TGA;
	}
	else if(extend == "psd")
	{
		return PSD;
	}
	else
	{
		return ETC;
	}
}

uint32_t TextureManager::LoadTexture(const std::string& path)
{
	if (nextTexture > 2024)
	{
		assert(0);
	}

	//一回読み込んだことがあるファイルはそのまま返す
	auto itr = find_if(textureDatas.begin(), textureDatas.end(), [&](std::pair<const std::string, std::unique_ptr<TextureData, std::default_delete<TextureData>>>& p)
		{
			return p.second->path == path;
		});

	if (itr == textureDatas.end())
	{
		std::unique_ptr<TextureData> data;

		data.reset(FromTextureData(path));
		data->textureHandle = nextTexture;
		data->path = path;

		textureDatas[path] = std::move(data);
		filePaths[nextTexture] = path;
		uint32_t handl = nextTexture;

		nextTexture++;

		return handl;
	}
	else
	{

		uint32_t modelHandle = textureDatas[path]->textureHandle;

		return modelHandle;
	}
}

void TextureManager::Initialize()
{
	directX12Core = DirectX12Core::GetInstance();

	// ヒープ設定
	textureHeapProp = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);

	filePaths.resize(static_cast<size_t>(2024));
}

TextureManager* TextureManager::GetInstance()
{
	if (!textureManager)
	{
		textureManager = new TextureManager();
	}

	return textureManager;
}

void TextureManager::Destroy()
{
	delete textureManager;
}

uint32_t TextureManager::Load(const std::string& path)
{
	return TextureManager::GetInstance()->LoadTexture(path);
}

TextureData* TextureManager::GetTextureData(uint32_t handle)
{
	return textureDatas[filePaths[handle]].get();
}

Microsoft::WRL::ComPtr<ID3D12Resource> TextureManager::CreateTexBuff(DirectX::TexMetadata& metadata, DirectX::ScratchImage& scratchImg)
{
	directX12Core->BeginCommand();

	std::vector<D3D12_SUBRESOURCE_DATA> textureSubresources;

	for (size_t i = 0; i < metadata.mipLevels; i++)
	{
		D3D12_SUBRESOURCE_DATA subresouce;

		subresouce.pData = scratchImg.GetImages()[i].pixels;
		subresouce.RowPitch = static_cast<LONG_PTR>(scratchImg.GetImages()[i].rowPitch);
		subresouce.SlicePitch = static_cast<LONG_PTR>(scratchImg.GetImages()[i].slicePitch);

		textureSubresources.push_back(subresouce);
	}

	Microsoft::WRL::ComPtr<ID3D12Resource> result;
	// リソース設定
	D3D12_RESOURCE_DESC textureResourceDesc = 
		CD3DX12_RESOURCE_DESC::Tex2D(metadata.format,
			metadata.width,
		static_cast<UINT>(metadata.height),
			static_cast<UINT16>(metadata.arraySize),
			static_cast<UINT16>(metadata.mipLevels));

	// テクスチャバッファの生成
	HRESULT hr = directX12Core->GetDevice()->CreateCommittedResource(
		&textureHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&textureResourceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(result.ReleaseAndGetAddressOf()));

	//テクスチャバッファにデータ転送
	directX12Core->GetDevice()->CreateCommittedResource(
		&textureHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&textureResourceDesc,
		D3D12_RESOURCE_STATE_COPY_DEST,
		nullptr,
		IID_PPV_ARGS(&result)
	);

	// ステージングバッファ準備
	UINT64 totalBytes;

	totalBytes = GetRequiredIntermediateSize(result.Get(), 0, static_cast<UINT>(textureSubresources.size()));

	Microsoft::WRL::ComPtr<ID3D12Resource> stagingBuffer;
	const auto heapProps = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	const auto resDesc = CD3DX12_RESOURCE_DESC::Buffer(totalBytes);
	hr = directX12Core->GetDevice()->CreateCommittedResource(
		&heapProps,
		D3D12_HEAP_FLAG_NONE,
		&resDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&stagingBuffer)
	);

	UpdateSubresources(directX12Core->GetCommandList().Get(), result.Get(), stagingBuffer.Get(), 0, 0, uint32_t(textureSubresources.size()), textureSubresources.data());

	// コピー後にはテクスチャとしてのステートへ.
	auto barrierTex = CD3DX12_RESOURCE_BARRIER::Transition(
		result.Get(),
		D3D12_RESOURCE_STATE_COPY_DEST,
		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE
	);
	directX12Core->GetCommandList()->ResourceBarrier(1, &barrierTex);

	directX12Core->ExecuteCommand(false);

	return result;
}

D3D12_GPU_DESCRIPTOR_HANDLE TextureManager::CreateShaderResourceView(ID3D12Resource* texBuff, DirectX::TexMetadata& metadata)
{
	// シェーダリソースビュー設定
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = metadata.format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = static_cast<UINT>(metadata.mipLevels);

	D3D12_GPU_DESCRIPTOR_HANDLE result{};

	result.ptr = directX12Core->GetSRVDescriptorHeap()->CreateSRV(srvDesc, texBuff);

	return result;
}

TextureData::~TextureData()
{
}
