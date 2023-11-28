#pragma once
#include<MaterialManager.h>

void CreateLaserDrawMaterial(MaterialManager* manager_,IAdapter* adapter_);

void CreateLaserEmitComputeMaterial(MaterialManager* manager_,IAdapter* adapter_);

void CreateLaserUpdateComputeMaterial(MaterialManager* manager_,IAdapter* adapter_);

void CreateLaserDrawArgumentUpdateComputeMaterial(MaterialManager* manager_,IAdapter* adapter_);

void CreateLaserFreeListInitComputeMaterial(MaterialManager* manager_,IAdapter* adapter_);