#pragma once

#include<MaterialManager.h>

void CreateAnimationMeshDrawMaterial(MaterialManager* manager_,IAdapter* adapter_);

void CreateAnimationMeshEmitComputeMaterial(MaterialManager* manager_,IAdapter* adapter_);

void CreateAnimationMeshUpdateComputeMaterial(MaterialManager* manager_,IAdapter* adapter_);

void CreateAnimationMeshDrawArgumentUpdateComputeMaterial(MaterialManager* manager_,IAdapter* adapter_);

void CreateAnimationMeshFreeListInitComputeMaterial(MaterialManager* manager_,IAdapter* adapter_);



void CreateMeshDrawMaterial(MaterialManager* manager_,IAdapter* adapter_);

void CreateMeshEmitComputeMaterial(MaterialManager* manager_,IAdapter* adapter_);

void CreateMeshUpdateComputeMaterial(MaterialManager* manager_,IAdapter* adapter_);

void CreateMeshDrawArgumentUpdateComputeMaterial(MaterialManager* manager_,IAdapter* adapter_);

void CreateMeshFreeListInitComputeMaterial(MaterialManager* manager_,IAdapter* adapter_);
