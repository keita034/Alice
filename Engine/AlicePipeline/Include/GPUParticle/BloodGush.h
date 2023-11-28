#pragma once

#include<MaterialManager.h>

void CreateBloodGushDrawMaterial(MaterialManager* manager_,IAdapter* adapter_);

void CreateBloodGushEmitComputeMaterial(MaterialManager* manager_,IAdapter* adapter_);

void CreateBloodGushUpdateComputeMaterial(MaterialManager* manager_,IAdapter* adapter_);

void CreateBloodGushDrawArgumentUpdateComputeMaterial(MaterialManager* manager_,IAdapter* adapter_);

void CreateBloodGushFreeListInitComputeMaterial(MaterialManager* manager_,IAdapter* adapter_);