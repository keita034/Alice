#include<BasePostEffect.h>

ISRVDescriptorHeap* BasePostEffect::sSrvHeap = nullptr;
IWindowsApp* BasePostEffect::sWindowsApp = nullptr;
ICommandList* BasePostEffect::sCmdList = nullptr;
IDevice* BasePostEffect::sMainDevice = nullptr;

BasePostEffect::~BasePostEffect()
{
}

void BasePostEffect::SSetDirectX12Core(ICommandList* cmdList_,ISRVDescriptorHeap* srvHeap_,IDevice* mainDevice_)
{
	sCmdList = cmdList_;
	sSrvHeap = srvHeap_;
	sMainDevice = mainDevice_;
}

void BasePostEffect::SSetWindowsApp(IWindowsApp* windowsApp_)
{
	sWindowsApp = windowsApp_;
}
