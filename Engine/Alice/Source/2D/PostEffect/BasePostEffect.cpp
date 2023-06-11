#include<BasePostEffect.h>

IDescriptorHeap* BasePostEffect::srvHeap = nullptr;
IWindowsApp* BasePostEffect::windowsApp = nullptr;

BasePostEffect::~BasePostEffect()
{
}

void BasePostEffect::SetSrvHeap(IDescriptorHeap* heap)
{
	srvHeap = heap;
}

void BasePostEffect::SetWindowsApp(IWindowsApp* windowsApp_)
{
	windowsApp = windowsApp_;
}
