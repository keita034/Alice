﻿#pragma warning(push)
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

std::unique_ptr<TextureManager> TextureManager::sTextureManager = nullptr;
DirectX12Core* TextureManager::sDirectX12Core = nullptr;
std::vector<std::string>TextureManager::sFilePaths;
std::unordered_map<std::string, std::unique_ptr<TextureData>> TextureManager::sTextureDatas;

void TextureManager::PLoadFile(const std::string& path_,  DirectX::TexMetadata& metadata_,  DirectX::ScratchImage& scratchImg_)
{
	wchar_t lWfilepath[256];
	HRESULT lResult = 0;

	switch (PGetFileType(path_))
	{
	case WIC:
		MultiByteToWideChar(CP_ACP, 0, path_.c_str(), -1, lWfilepath, _countof(lWfilepath));
		// WICテクスチャのロード
		lResult = LoadFromWICFile(
			lWfilepath,
			DirectX::WIC_FLAGS_NONE,
			&metadata_, scratchImg_);
		assert(SUCCEEDED(lResult));
		break;

	case TGA:
		MultiByteToWideChar(CP_ACP, 0, path_.c_str(), -1, lWfilepath, _countof(lWfilepath));
		// TGAテクスチャのロード
		lResult = LoadFromTGAFile(
			lWfilepath,
			&metadata_, scratchImg_);
		assert(SUCCEEDED(lResult));
		break;

	case PSD:
	{
		std::string texPath = AliceFunctionUtility::ReplaceExtension(path_, "tga");

		MultiByteToWideChar(CP_ACP, 0, texPath.c_str(), -1, lWfilepath, _countof(lWfilepath));

		// TGAテクスチャのロード
		lResult = LoadFromTGAFile(
			lWfilepath,
			&metadata_, scratchImg_);
		assert(SUCCEEDED(lResult));
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

std::unique_ptr<TextureData> TextureManager::PFromTextureData(const std::string& path_)
{
	std::unique_ptr<TextureData> lResult = std::make_unique<TextureData>();

	DirectX::TexMetadata lMetadata{};
	DirectX::ScratchImage lScratchImg{};
	DirectX::ScratchImage lMipChain{};

	lResult->srvHeap = sDirectX12Core->GetSRVDescriptorHeap()->GetHeap();

	PLoadFile(path_, lMetadata, lScratchImg);

	//ミップマップ生成
	HRESULT lHresult = GenerateMipMaps(
		lScratchImg.GetImages(), lScratchImg.GetImageCount(), lScratchImg.GetMetadata(),
		DirectX::TEX_FILTER_DEFAULT, 0, lMipChain);

	if (SUCCEEDED(lHresult))
	{
		lScratchImg = std::move(lMipChain);
		lMetadata = lScratchImg.GetMetadata();
	}

	//読み込んだディフューズテクスチャをSRGBとして扱う
	lMetadata.format = DirectX::MakeSRGB(lMetadata.format);

	lResult->texBuff = PCreateTexBuff(lMetadata, lScratchImg);

	lResult->gpuHandle = PCreateShaderResourceView(lResult->texBuff.Get(), lMetadata);

	lResult->width = lMetadata.width;
	lResult->height = lMetadata.height;

	return std::move(lResult);
}

TextureManager::ImgFileType TextureManager::PGetFileType(const std::string& path_)
{
	std::string lExtend = AliceFunctionUtility::FileExtension(path_);
	if (lExtend == "png" ||
		lExtend == "bmp" ||
		lExtend == "gif" ||
		lExtend == "tiff"||
		lExtend == "jpeg"||
		lExtend == "jpg")
	{
		return WIC;
	}
	else if (lExtend == "tga")
	{
		return TGA;
	}
	else if(lExtend == "psd")
	{
		return PSD;
	}
	else
	{
		return ETC;
	}
}

uint32_t TextureManager::LoadTexture(const std::string& path_)
{
	if (nextTexture > 2024)
	{
		assert(0);
	}

	//一回読み込んだことがあるファイルはそのまま返す
	auto textureItr = find_if(sTextureDatas.begin(), sTextureDatas.end(), [&](std::pair<const std::string, std::unique_ptr<TextureData, std::default_delete<TextureData>>>& texture)
		{
			return texture.second->path == path_;
		});

	if (textureItr == sTextureDatas.end())
	{
		std::unique_ptr<TextureData> lData;

		lData = PFromTextureData(path_);
		lData->textureHandle = nextTexture;
		lData->path = path_;

		sTextureDatas[path_] = std::move(lData);
		sFilePaths[nextTexture] = path_;
		uint32_t lHandl = nextTexture;

		nextTexture++;

		return lHandl;
	}
	else
	{

		uint32_t lModelHandle = sTextureDatas[path_]->textureHandle;

		return lModelHandle;
	}
}

void TextureManager::Initialize()
{
	// ヒープ設定
	textureHeapProp = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);

	sFilePaths.resize(static_cast<size_t>(2024));
}

TextureManager* TextureManager::SGetInstance()
{
	if (!sTextureManager)
	{
		TextureManager* lTextureManager = new TextureManager();
		sTextureManager.reset(lTextureManager);
	}

	return sTextureManager.get();
}

uint32_t TextureManager::SLoad(const std::string& path_)
{
	return TextureManager::SGetInstance()->LoadTexture(path_);
}

TextureData* TextureManager::SGetTextureData(uint32_t handle_)
{
	return sTextureDatas[sFilePaths[handle_]].get();
}

void TextureManager::SSetDirectX12Core(DirectX12Core* directX12Core_)
{
	sDirectX12Core = directX12Core_;
}

Microsoft::WRL::ComPtr<ID3D12Resource> TextureManager::PCreateTexBuff(DirectX::TexMetadata& metadata_, DirectX::ScratchImage& scratchImg_)
{
	sDirectX12Core->BeginCommand();

	std::vector<D3D12_SUBRESOURCE_DATA> lTextureSubresources;

	for (size_t i = 0; i < metadata_.mipLevels; i++)
	{
		D3D12_SUBRESOURCE_DATA lSubresouce;

		lSubresouce.pData = scratchImg_.GetImages()[i].pixels;
		lSubresouce.RowPitch = static_cast<LONG_PTR>(scratchImg_.GetImages()[i].rowPitch);
		lSubresouce.SlicePitch = static_cast<LONG_PTR>(scratchImg_.GetImages()[i].slicePitch);

		lTextureSubresources.push_back(lSubresouce);
	}

	Microsoft::WRL::ComPtr<ID3D12Resource> lResult;
	// リソース設定
	D3D12_RESOURCE_DESC lTextureResourceDesc = 
		CD3DX12_RESOURCE_DESC::Tex2D(
			metadata_.format,
			metadata_.width,
			static_cast<UINT>(metadata_.height),
			static_cast<UINT16>(metadata_.arraySize),
			static_cast<UINT16>(metadata_.mipLevels));


	//テクスチャバッファにデータ転送
	sDirectX12Core->GetDevice()->CreateCommittedResource(
		&textureHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&lTextureResourceDesc,
		D3D12_RESOURCE_STATE_COPY_DEST,
		nullptr,
		IID_PPV_ARGS(&lResult)
	);

	// ステージングバッファ準備
	UINT64 lTotalBytes;

	lTotalBytes = GetRequiredIntermediateSize(lResult.Get(), 0, static_cast<UINT>(lTextureSubresources.size()));

	Microsoft::WRL::ComPtr<ID3D12Resource> lStagingBuffer;
	auto lHeapProps = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	auto lResDesc = CD3DX12_RESOURCE_DESC::Buffer(lTotalBytes);
	HRESULT lHresult = sDirectX12Core->GetDevice()->CreateCommittedResource(
		&lHeapProps,
		D3D12_HEAP_FLAG_NONE,
		&lResDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&lStagingBuffer));

	if (FAILED(lHresult))
	{
		assert(0);
	}

	UpdateSubresources(sDirectX12Core->GetCommandList(), lResult.Get(), lStagingBuffer.Get(), 0, 0, static_cast<uint32_t>(lTextureSubresources.size()), lTextureSubresources.data());

	// コピー後にはテクスチャとしてのステートへ.
	auto lBarrierTex = CD3DX12_RESOURCE_BARRIER::Transition(
		lResult.Get(),
		D3D12_RESOURCE_STATE_COPY_DEST,
		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE
	);

	sDirectX12Core->GetCommandList()->ResourceBarrier(1, &lBarrierTex);

	sDirectX12Core->ExecuteCommand(false);

	return lResult;
}

D3D12_GPU_DESCRIPTOR_HANDLE TextureManager::PCreateShaderResourceView(ID3D12Resource* texBuff, const DirectX::TexMetadata& metadata_)
{
	// シェーダリソースビュー設定
	D3D12_SHADER_RESOURCE_VIEW_DESC lSrvDesc{};
	lSrvDesc.Format = metadata_.format;
	lSrvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	lSrvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	lSrvDesc.Texture2D.MipLevels = static_cast<UINT>(metadata_.mipLevels);

	D3D12_GPU_DESCRIPTOR_HANDLE lResult{};

	lResult.ptr = sDirectX12Core->GetSRVDescriptorHeap()->CreateSRV(lSrvDesc, texBuff);

	return lResult;
}

TextureData::~TextureData()
{
}
