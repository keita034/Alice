#include<Light.h>

IDevice* Light::sMainDevice = nullptr;
AliceMathF::Vector3* Light::lightdirPtr = nullptr;
AliceMathF::Vector4* Light::lightcolorPtr = nullptr;

#include<Defined.h>

ALICE_SUPPRESS_WARNINGS_BEGIN

#include<directx/d3d12.h>

ALICE_SUPPRESS_WARNINGS_END

void Light::Initialize()
{
	//定数バッファ
	constBuff = CreateUniqueConstantBuffer(sizeof(LightConstBuffData));

	//定数バッファへデータ転送
	TransferConstBuffer();

	if ( !lightdirPtr )
	{
		lightdirPtr = &lightdir;
	}
	if ( !lightcolorPtr )
	{
		lightcolorPtr = &lightcolor;
	}
}

void Light::TransferConstBuffer(){
	constMap.lightv = -lightdir;
	constMap.lightcolor = lightcolor;

	constBuff->Update(&constMap);
}

void Light::SetLightDir(const AliceMathF::Vector3& lightdir_)
{
	//正規化してセット
	lightdir = lightdir_.Normal();
	lightdirPtr = &lightdir;
	dirty = true;
}

void Light::SetLightColor(const AliceMathF::Vector4& lightcolor_)
{
	lightcolor = lightcolor_;
	lightcolorPtr = &lightcolor;
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

void Light::SetConstBufferView(ICommandList* cmdList_, uint32_t rootParameterIndex_)const
{
	ID3D12GraphicsCommandList* lCmdList = cmdList_->GetGraphicCommandList();

	//定数バッファビューをセット
	lCmdList->SetGraphicsRootConstantBufferView(rootParameterIndex_, constBuff->GetAddress());
}

void Light::SSetDevice(IDevice* device_)
{
	sMainDevice = device_;
}

AliceMathF::Vector3* Light::SGetLightDirPtr()
{
	return lightdirPtr;
}

AliceMathF::Vector4* Light::SGetLightColorPtr()
{
	return lightcolorPtr;
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