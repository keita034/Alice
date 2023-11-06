#pragma once
#include<Device.h>

class BasePipelineState
{
public:

	BasePipelineState() = default;
	virtual ~BasePipelineState() = default;

	static void SSetDevice(IDevice* device_);

	static void Finalize();

protected:

	static IDevice* sMainDevice;
};