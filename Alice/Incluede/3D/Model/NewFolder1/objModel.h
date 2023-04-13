#pragma once
#include"Model.h"
#include"DefaultMaterial.h"

class objModel
{

public:

	/// <summary>
	/// オブジェクト生成
	/// </summary>
	static void Create(const char* filePath, bool smoothing, ModelData* data);

private:

	static void LoadMaterial(const std::string& directoryPath,const std::string& filename, ModelData* data);
	static void LoadTexture(const std::string& directoryPath, const std::string& filename, ModelData* data);

	//スムージング頂点生成
	static void CalculateSmoothedVertexNormals(ModelData* data);
};