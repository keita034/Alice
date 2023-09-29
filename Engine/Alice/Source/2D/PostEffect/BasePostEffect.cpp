#include<BasePostEffect.h>

ISRVDescriptorHeap* BasePostEffect::sSrvHeap = nullptr;
IWindowsApp* BasePostEffect::sWindowsApp = nullptr;
ICommandList* BasePostEffect::sCmdList = nullptr;
IDevice* BasePostEffect::sDevice = nullptr;

BasePostEffect::~BasePostEffect()
{
}

void BasePostEffect::SSetDirectX12Core(DirectX12Core* directX12Core_)
{
	sCmdList = directX12Core_->GetCommandList();
	sSrvHeap = directX12Core_->GetSRVDescriptorHeap();
	sDevice = directX12Core_->GetDevice();
}

void BasePostEffect::SSetWindowsApp(IWindowsApp* windowsApp_)
{
	sWindowsApp = windowsApp_;
}
