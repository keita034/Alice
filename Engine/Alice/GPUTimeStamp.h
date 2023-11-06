#pragma once

#include<Defined.h>

ALICE_SUPPRESS_WARNINGS_BEGIN

#include<directx/d3d12.h>
#include<wrl.h>

ALICE_SUPPRESS_WARNINGS_END

class GPUTimeStamp
{
private:

	Microsoft::WRL::ComPtr<ID3D12QueryHeap>heap;

	Microsoft::WRL::ComPtr<ID3D12Resource>resource;

public:

	void Initialize();
	void Update();
	void Finalize();
	void Draw();

};

