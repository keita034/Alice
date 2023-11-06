#pragma once

#include<MaterialManager.h>

void CreateBasicDrawMaterial(MaterialManager* manager_,IAdapter* adapter_);

void CreateBasicEmitComputeMaterial(MaterialManager* manager_,IAdapter* adapter_);

void CreateBasicUpdateComputeMaterial(MaterialManager* manager_,IAdapter* adapter_);

void CreateBasicDrawArgumentUpdateComputeMaterial(MaterialManager* manager_,IAdapter* adapter_);

void CreateBasicFreeListInitComputeMaterial(MaterialManager* manager_,IAdapter* adapter_);