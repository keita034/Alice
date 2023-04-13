#include "AliceUtility.h"

ConstBuffDataMaterial ModelMaterial::GetConstBuffMaterial()
{
	ConstBuffDataMaterial rsult;
	rsult.ambient = ambient;
	rsult.diffuse = diffuse;
	rsult.specular = specular;
	rsult.alpha = alpha;
	return rsult;
}

ComputeRelation::ComputeRelation()
{
}

ComputeRelation::~ComputeRelation()
{
}

TextureData::~TextureData()
{
}
