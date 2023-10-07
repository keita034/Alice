#include "BasePipelineState.h"

IDevice* BasePipelineState::sDevice = nullptr;

void BasePipelineState::SSetDevice(IDevice* device_)
{
	sDevice = device_;
}
