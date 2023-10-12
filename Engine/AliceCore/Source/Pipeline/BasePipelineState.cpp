#include "BasePipelineState.h"

IDevice* BasePipelineState::sMainDevice = nullptr;

void BasePipelineState::SSetDevice(IDevice* device_)
{
	sMainDevice = device_;
}
