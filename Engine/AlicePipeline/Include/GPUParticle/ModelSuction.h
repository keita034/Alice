#pragma once

#include<MaterialManager.h>

void CreateModelSuctionDrawMaterial(MaterialManager* manager_,IAdapter* adapter_);

void CreateModelSuctionEmitComputeMaterial(MaterialManager* manager_,IAdapter* adapter_);

void CreateModelSuctionUpdateComputeMaterial(MaterialManager* manager_,IAdapter* adapter_);

void CreateModelSuctionDrawArgumentUpdateComputeMaterial(MaterialManager* manager_,IAdapter* adapter_);

void CreateModelSuctionFreeListInitComputeMaterial(MaterialManager* manager_,IAdapter* adapter_);