#pragma once

#include<MaterialManager.h>

void CreateFireDrawMaterial(MaterialManager* manager_,IAdapter* adapter_);

void CreateFireEmitComputeMaterial(MaterialManager* manager_,IAdapter* adapter_);

void CreateFireUpdateComputeMaterial(MaterialManager* manager_,IAdapter* adapter_);

void CreateFireDrawArgumentUpdateComputeMaterial(MaterialManager* manager_,IAdapter* adapter_);

void CreateFireFreeListInitComputeMaterial(MaterialManager* manager_,IAdapter* adapter_);