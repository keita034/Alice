#pragma once
#pragma warning(push)
#pragma warning(disable: 4365)
#pragma warning(disable: 4514)
#pragma warning(disable: 4619)
#pragma warning(disable: 4668)
#pragma warning(disable: 5039)

#include<fstream>

#pragma warning(pop)

#include"AliceModel.h"

class AliceFileStream
{

	static std::string buff;

	static std::istringstream lineData;

	static std::array<char, 256>buffC;

	static std::string directoryPath;

	friend class AliceModelData;
	friend class AliceModel;
	friend class ModelMesh;

public:
	static bool LoadAlicePolygonData(const std::string& path, AliceModelData* model);

	static bool LoadAlicePolygonBinaryData(const std::string& path, AliceModelData* model);

	static bool LoadAliceMotionBinaryData(const std::string& path, MotionData* motion);

	static bool LoadAliceMotionData(const std::string& path, MotionData* motion);

private:

#pragma region モデルデータ

#pragma region ASCII

	static void ReadModelData(std::stringstream& data, std::string& str, AliceModelData* model);

	static void ReadMeshsData(std::stringstream& data, std::string& str, AliceModelData* model);

	static void ReadMeshData(std::stringstream& data, std::string& str, ModelMesh& mesh, std::vector<Node>& nodes);

	static void ReadHesderData(std::stringstream& data, std::string& str, AliceModelData* model);

	static void ReadBoneIndex(const std::string& strInd, std::array<UINT, MAX_BONE_INDICES>& boneIndex);

	static void ReadBoneWeight(const std::string& strWei, std::array<float, MAX_BONE_INDICES>& boneWeight);

	static void ReadBoneData(const std::string& strbone, Bone& bone);

	static void ReadMaterial(const std::string& strMate, ModelMaterial& modelMaterial);

	static void ReadNodesData(std::stringstream& data, std::string& str, AliceModelData* model);

	static void ReadNodeData(std::stringstream& data, std::string& str, Node& node, AliceModelData* model);

	static void ReadParentData(const std::string& name, Node& node, std::vector<Node>& nodes);

#pragma endregion

#pragma region バイナリ

	static bool ReadHesderBinaryData(AliceModelData* model, FILE* fp);

	static bool ReadNodesBinaryData(AliceModelData* model, FILE* fp);

	static bool ReadMeshBinaryData(AliceModelData* model, FILE* fp);

#pragma endregion

#pragma endregion

#pragma region アニメーションデータ

#pragma region アスキー

	static void ReadMotionData(std::stringstream& data, std::string& str, MotionData* motionData);

	static void ReadMotionHesderData(std::stringstream& data, std::string& str, MotionData* motion);

	static void ReadMotionNodesData(std::stringstream& data, std::string& str, MotionData* motion);
	static void ReadMotionNodeData(std::stringstream& data, std::string& str, MotionNode& node);

#pragma endregion

#pragma region バイナリ

	static bool ReadMotionNodeBinaryData(MotionNode* node, FILE* fp);

#pragma endregion

#pragma endregion

#pragma region その他

	static void ReadString(const std::string& strData, std::string& str);
	static void ReadFlag(const std::string& strFlag, bool& flag);
	static void ReadNumber(const std::string& strNum, float& num);
	static void ReadNumber(const std::string& strNum, uint32_t& num);
	static void ReadVertex(const std::string& strVer, PosNormUvTangeColSkin& ver);
	static void ReadVector3(const std::string& strVec, AliceMathF::Vector3& ver);
	static void ReadVector3(const std::string& strVec, AliceMathF::Vector4& ver);
	static void ReadVector2(const std::string& strVec, AliceMathF::Vector2& ver);
	static void ReadVector4(const std::string& strVec, AliceMathF::Vector4& ver);
	static void ReadQuaternion(const std::string& strVec, AliceMathF::Quaternion& ver);
	static void ReadMatrix(const std::string& strMat, AliceMathF::Matrix4& mat);
	static float Atof(const std::string& str);
	static void ReadVectorKey(const std::string& strVer, VectorKey& ver);
	static void ReadQuaternionKey(const std::string& strVer, QuaternionKey& ver);

#pragma endregion

};