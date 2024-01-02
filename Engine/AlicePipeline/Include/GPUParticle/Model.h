#pragma once
#include<MaterialManager.h>

void CreateAnimationModelDrawMaterial(MaterialManager* manager_,IAdapter* adapter_);

void CreateAnimationModelEmitComputeMaterial(MaterialManager* manager_,IAdapter* adapter_);

void CreateAnimationModelUpdateComputeMaterial(MaterialManager* manager_,IAdapter* adapter_);

void CreateAnimationModelDrawArgumentUpdateComputeMaterial(MaterialManager* manager_,IAdapter* adapter_);

void CreateAnimationModelFreeListInitComputeMaterial(MaterialManager* manager_,IAdapter* adapter_);

void CreateAnimationModelDrawListReleaseComputeMaterial(MaterialManager* manager_,IAdapter* adapter_);



void CreateModelDrawMaterial(MaterialManager* manager_,IAdapter* adapter_);

void CreateModelEmitComputeMaterial(MaterialManager* manager_,IAdapter* adapter_);

void CreateModelUpdateComputeMaterial(MaterialManager* manager_,IAdapter* adapter_);

void CreateModelDrawArgumentUpdateComputeMaterial(MaterialManager* manager_,IAdapter* adapter_);

void CreateModelFreeListInitComputeMaterial(MaterialManager* manager_,IAdapter* adapter_);
