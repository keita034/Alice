#include<BasePostEffect.h>

DescriptorHeap* BasePostEffect::srvHeap = nullptr;

BasePostEffect::~BasePostEffect()
{
}

void BasePostEffect::SetSrvHeap(DescriptorHeap* heap)
{
	srvHeap = heap;
}
