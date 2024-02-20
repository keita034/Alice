#include<Defined.h>

ALICE_SUPPRESS_WARNINGS_BEGIN

#include<directx/d3dx12.h>

ALICE_SUPPRESS_WARNINGS_END

#include "TextureManager.h"
#include<StringUtility.h>
#include<FileUtility.h>

std::unique_ptr<TextureManager> TextureManager::sTextureManager = nullptr;
std::vector<std::string>TextureManager::sFilePaths;
std::unordered_map<std::string,std::unique_ptr<TextureData>> TextureManager::sTextureDatas;
IMultiAdapters* TextureManager::multiAdapters;
ISwapChain* TextureManager::swapChain;
ISRVDescriptorHeap* TextureManager::srvDescriptorHeap;

void TextureManager::PLoadFile(const std::string& path_,DirectX::TexMetadata& metadata_,DirectX::ScratchImage& scratchImg_)
{
	std::wstring lWfilepath = AliceUtility::String::StringToWstring(path_);
	HRESULT lResult = 0;

	switch ( PGetFileType(path_) )
	{
	case WIC:
		// WICテクスチャのロード
		lResult = LoadFromWICFile(
			lWfilepath.c_str(),
			DirectX::WIC_FLAGS_NONE,
			&metadata_,scratchImg_);
		assert(SUCCEEDED(lResult));
		break;

	case TGA:
		// TGAテクスチャのロード
		lResult = LoadFromTGAFile(
			lWfilepath.c_str(),
			&metadata_,scratchImg_);
		assert(SUCCEEDED(lResult));
		break;

	case PSD:
	{
		std::string texPath = AliceUtility::Fille::ReplaceExtension(path_,"tga");

		// TGAテクスチャのロード
		lResult = LoadFromTGAFile(
			lWfilepath.c_str(),
			&metadata_,scratchImg_);
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

std::unique_ptr<TextureData> TextureManager::PFromTextureData(const std::string& path_,AdaptersIndex index_)
{
	std::unique_ptr<TextureData> lResult = std::make_unique<TextureData>();

	DirectX::TexMetadata lMetadata{};
	DirectX::ScratchImage lScratchImg{};
	DirectX::ScratchImage lMipChain{};

	IAdapter* lAdapter = multiAdapters->GetAdapter(index_);
	lResult->srvHeap = lAdapter->GetSRVDescriptorHeap()->GetHeap();

	PLoadFile(path_,lMetadata,lScratchImg);

	//ミップマップ生成
	HRESULT lHresult = GenerateMipMaps(
		lScratchImg.GetImages(),lScratchImg.GetImageCount(),lScratchImg.GetMetadata(),
		DirectX::TEX_FILTER_DEFAULT,0,lMipChain);

	if ( SUCCEEDED(lHresult) )
	{
		lScratchImg = std::move(lMipChain);
		lMetadata = lScratchImg.GetMetadata();
	}

	//読み込んだディフューズテクスチャをSRGBとして扱う
	lMetadata.format = DirectX::MakeSRGB(lMetadata.format);

	lResult->texBuff = PCreateTexBuff(lMetadata,lScratchImg,index_);

	lResult->gpuHandle = PCreateShaderResourceView(lResult->texBuff.Get(),lMetadata,index_);

	lResult->width = lMetadata.width;
	lResult->height = lMetadata.height;

	return std::move(lResult);
}

TextureManager::ImgFileType TextureManager::PGetFileType(const std::string& path_)
{
	std::string lExtend = AliceUtility::Fille::FileExtension(path_);
	if ( lExtend == "png" ||
		lExtend == "bmp" ||
		lExtend == "gif" ||
		lExtend == "tiff" ||
		lExtend == "jpeg" ||
		lExtend == "jpg" )
	{
		return WIC;
	}
	else if ( lExtend == "tga" )
	{
		return TGA;
	}
	else if ( lExtend == "psd" )
	{
		return PSD;
	}
	else
	{
		return ETC;
	}
}

uint32_t TextureManager::LoadTexture(const std::string& path_,AdaptersIndex index_)
{
	if ( nextTexture > 2024 )
	{
		assert(0);
	}

	//一回読み込んだことがあるファイルはそのまま返す
	auto textureItr = find_if(sTextureDatas.begin(),sTextureDatas.end(),[ & ] (std::pair<const std::string,std::unique_ptr<TextureData,std::default_delete<TextureData>>>& texture)
		{
			return texture.second->path == path_;
		});

	if ( textureItr == sTextureDatas.end() )
	{
		std::unique_ptr<TextureData> lData;

		lData = PFromTextureData(path_,index_);
		lData->textureHandle = nextTexture;
		lData->path = path_;
		lData->resourceState = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;

		sTextureDatas[ path_ ] = std::move(lData);
		sFilePaths[ nextTexture ] = path_;
		uint32_t lHandl = nextTexture;

		nextTexture++;

		return lHandl;
	}
	else
	{

		uint32_t lModelHandle = sTextureDatas[ path_ ]->textureHandle;

		return lModelHandle;
	}
}

void TextureManager::Initialize()
{
	// ヒープ設定
	textureHeapProp = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);

	sFilePaths.resize(static_cast< size_t >( 2024 ));
}

TextureManager* TextureManager::SGetInstance()
{
	if ( !sTextureManager )
	{
		TextureManager* lTextureManager = new TextureManager();
		sTextureManager.reset(lTextureManager);
	}

	return sTextureManager.get();
}

uint32_t TextureManager::SLoad(const std::string& path_,AdaptersIndex index_)
{
	return TextureManager::SGetInstance()->LoadTexture(path_,index_);
}

TextureData* TextureManager::SGetTextureData(uint32_t handle_)
{
	return sTextureDatas[ sFilePaths[ handle_ ] ].get();
}

void TextureManager::SSetDirectX12Core(IMultiAdapters* multiAdapters_,ISwapChain* swapChain_,ISRVDescriptorHeap* srvDescriptorHeap_)
{
	multiAdapters = multiAdapters_;
	swapChain = swapChain_;
	srvDescriptorHeap = srvDescriptorHeap_;
}

void TextureManager::Finalize()
{
	sTextureDatas.clear();
	sTextureManager.release();
}

Microsoft::WRL::ComPtr<ID3D12Resource> TextureManager::PCreateTexBuff(DirectX::TexMetadata& metadata_,DirectX::ScratchImage& scratchImg_,AdaptersIndex index_)
{
	IAdapter* lAdapter = multiAdapters->GetAdapter(index_);
	ID3D12Device* lDevice = lAdapter->GetDevice()->Get();
	ID3D12GraphicsCommandList* lCommandList = lAdapter->GetGraphicCommandList();
	size_t bbIndex = swapChain->GetCurrentBackBufferIndex();

	lAdapter->GraphicCommandListReset(bbIndex);

	std::vector<D3D12_SUBRESOURCE_DATA> lTextureSubresources;

	for ( size_t i = 0; i < metadata_.mipLevels; i++ )
	{
		D3D12_SUBRESOURCE_DATA lSubresouce{};

		lSubresouce.pData = scratchImg_.GetImages()[ i ].pixels;
		lSubresouce.RowPitch = static_cast< LONG_PTR >( scratchImg_.GetImages()[ i ].rowPitch );
		lSubresouce.SlicePitch = static_cast< LONG_PTR >( scratchImg_.GetImages()[ i ].slicePitch );

		lTextureSubresources.push_back(lSubresouce);
	}

	Microsoft::WRL::ComPtr<ID3D12Resource> lResult;
	// リソース設定
	D3D12_RESOURCE_DESC lTextureResourceDesc =
		CD3DX12_RESOURCE_DESC::Tex2D(
			metadata_.format,
			metadata_.width,
			static_cast< UINT >( metadata_.height ),
			static_cast< UINT16 >( metadata_.arraySize ),
			static_cast< UINT16 >( metadata_.mipLevels ));


	//テクスチャバッファにデータ転送

	lDevice->CreateCommittedResource(
		&textureHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&lTextureResourceDesc,
		D3D12_RESOURCE_STATE_COPY_DEST,
		nullptr,
		IID_PPV_ARGS(&lResult)
	);

	// ステージングバッファ準備
	UINT64 lTotalBytes;

	lTotalBytes = GetRequiredIntermediateSize(lResult.Get(),0,static_cast< UINT >( lTextureSubresources.size() ));

	Microsoft::WRL::ComPtr<ID3D12Resource> lStagingBuffer;
	auto lHeapProps = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	auto lResDesc = CD3DX12_RESOURCE_DESC::Buffer(lTotalBytes);
	HRESULT lHresult = lDevice->CreateCommittedResource(
		&lHeapProps,
		D3D12_HEAP_FLAG_NONE,
		&lResDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&lStagingBuffer));

	if ( FAILED(lHresult) )
	{
		assert(0);
	}

	UpdateSubresources(lCommandList,lResult.Get(),lStagingBuffer.Get(),0,0,static_cast< uint32_t >( lTextureSubresources.size() ),lTextureSubresources.data());

	// コピー後にはテクスチャとしてのステートへ.
	auto lBarrierTex = CD3DX12_RESOURCE_BARRIER::Transition(
		lResult.Get(),
		D3D12_RESOURCE_STATE_COPY_DEST,
		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE
	);

	lCommandList->ResourceBarrier(1,&lBarrierTex);

	lAdapter->GraphicCommandListExecute();
	lAdapter->GraphicWaitPreviousFrame();

	return lResult;
}

D3D12_GPU_DESCRIPTOR_HANDLE TextureManager::PCreateShaderResourceView(ID3D12Resource* texBuff,const DirectX::TexMetadata& metadata_,AdaptersIndex index_)
{
	IAdapter* lAdapter = multiAdapters->GetAdapter(index_);
	ISRVDescriptorHeap* lSRVDescriptorHeap = lAdapter->GetSRVDescriptorHeap();

	// シェーダリソースビュー設定
	D3D12_SHADER_RESOURCE_VIEW_DESC lSrvDesc{};
	lSrvDesc.Format = metadata_.format;
	lSrvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	lSrvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	lSrvDesc.Texture2D.MipLevels = static_cast< UINT >( metadata_.mipLevels );

	D3D12_GPU_DESCRIPTOR_HANDLE lResult{};

	lResult.ptr = lSRVDescriptorHeap->CreateSRV(lSrvDesc,texBuff);

	return lResult;
}