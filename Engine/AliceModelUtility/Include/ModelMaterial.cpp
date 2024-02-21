#include "ModelMaterial.h"

ConstBuffDataMaterial ModelMaterial::GetConstBuffMaterial()
{
	ConstBuffDataMaterial lRsult;
	lRsult.ambient = ambient;
	lRsult.diffuse = diffuse;
	lRsult.specular = specular;
	lRsult.alpha = alpha;
	return lRsult;
}