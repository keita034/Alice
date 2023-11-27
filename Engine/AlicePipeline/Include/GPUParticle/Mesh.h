#pragma once

#include<MaterialManager.h>

void CreateMeshDrawMaterial(MaterialManager* manager_,IAdapter* adapter_);

void CreateMeshEmitComputeMaterial(MaterialManager* manager_,IAdapter* adapter_);

void CreateMeshUpdateComputeMaterial(MaterialManager* manager_,IAdapter* adapter_);

void CreateMeshDrawArgumentUpdateComputeMaterial(MaterialManager* manager_,IAdapter* adapter_);

void CreateMeshFreeListInitComputeMaterial(MaterialManager* manager_,IAdapter* adapter_);