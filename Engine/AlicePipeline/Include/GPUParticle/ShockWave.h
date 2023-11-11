#pragma once
#include<MaterialManager.h>

void CreateShockWaveDrawMaterial(MaterialManager* manager_,IAdapter* adapter_);

void CreateShockWaveEmitComputeMaterial(MaterialManager* manager_,IAdapter* adapter_);

void CreateShockWaveUpdateComputeMaterial(MaterialManager* manager_,IAdapter* adapter_);

void CreateShockWaveDrawArgumentUpdateComputeMaterial(MaterialManager* manager_,IAdapter* adapter_);

void CreateShockWaveFreeListInitComputeMaterial(MaterialManager* manager_,IAdapter* adapter_);