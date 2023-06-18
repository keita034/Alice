#include<Light.h>
#include<DirectX12Core.h>

ID3D12Device* Light::device = nullptr;

void Light::Initialize()
{
	//定数バッファ
	constBuff = CreateUniqueConstantBuffer(sizeof(LightConstBuffData));

	//定数バッファへデータ転送
	TransferConstBuffer();
}

void Light::TransferConstBuffer(){
	constMap.lightv = -lightdir;
	constMap.lightcolor = lightcolor;

	constBuff->Update(&constMap);
}

void Light::SetLightDir(const AliceMathF::Vector3& lightdir_)
{
	//正規化してセット
	lightdir = lightdir_.Normalization();
	dirty = true;
}

void Light::SetLightColor(const AliceMathF::Vector4& lightcolor_)
{
	lightcolor = lightcolor_;
	dirty = true;
}

void Light::Update()
{
	//値の更新があった時だけ定数バッファに転送する
	if (dirty)
	{
		TransferConstBuffer();
		dirty = false;
	}
}

void Light::SetConstBufferView(ID3D12GraphicsCommandList* cmdList_, uint32_t rootParameterIndex_)const
{
	//定数バッファビューをセット
	cmdList_->SetGraphicsRootConstantBufferView(rootParameterIndex_, constBuff->GetAddress());
}

void Light::SSetDevice(ID3D12Device* device_)
{
	device = device_;
}

std::unique_ptr<Light> CreateUniqueLight()
{
	std::unique_ptr<Light>lLight = std::make_unique<Light>();
	lLight->Initialize();
	return std::move(lLight);
}

std::shared_ptr<Light> CreateSharedLight()
{
	std::shared_ptr<Light> lLight = std::make_unique<Light>();
	lLight->Initialize();
	return lLight;
}
