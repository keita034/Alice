#pragma once
#pragma warning(push)
#pragma warning(disable: 4365)
#pragma warning(disable: 4514)
#pragma warning(disable: 4619)
#pragma warning(disable: 4668)
#pragma warning(disable: 4710)
#pragma warning(disable: 5039)

#include<fstream>

#pragma warning(pop)

#include<AliceModel.h>

class AliceFileStream
{

	static std::string sBbuff;

	static std::istringstream sLineData;

	static std::array<char, 256>cBuffC;

	static std::string sDirectoryPath;

	friend class AliceModelData;
	friend class AliceModel;
	friend class ModelMesh;

public:
	static bool SLoadAlicePolygonData(const std::string& path_, AliceModelData* model_);

	static bool SLoadAlicePolygonBinaryData(const std::string& path_, AliceModelData* model_);

	static bool SLoadAliceMotionBinaryData(const std::string& path_, MotionData* motion_);

	static bool SLoadAliceMotionData(const std::string& path_, MotionData* motion_);

private:

#pragma region モデルデータ

#pragma region ASCII

	static void SPReadModelData(std::stringstream& data_, std::string& str_, AliceModelData* model_);

	static void SPReadMeshsData(std::stringstream& data_, std::string& str_, AliceModelData* model_);

	static void SPReadMeshData(std::stringstream& data_, std::string& str_, std::unique_ptr<ModelMesh>& mesh_, std::vector<Node>& nodes_);

	static void SPReadHesderData(std::stringstream& data_, std::string& str_, AliceModelData* model_);

	static void SPReadBoneIndex(const std::string& strIndex_, std::array<uint32_t, MAX_BONE_INDICES>& boneIndex_);

	static void SPReadBoneWeight(const std::string& strWeight_, std::array<float, MAX_BONE_INDICES>& boneWeight_);

	static void SPReadBoneData(const std::string& strBone_, Bone& bone_);

	static void SPReadMaterial(const std::string& strMaterial_, ModelMaterial& modelMaterial_);

	static void SPReadNodesData(std::stringstream& data_, std::string& str_, AliceModelData* model_);

	static void SPReadNodeData(std::stringstream& data_, std::string& str_, Node& node_, AliceModelData* model_);

	static void SPReadParentData(const std::string& name_, Node& node_, std::vector<Node>& nodes_);

#pragma endregion

#pragma region バイナリ

	static bool SPReadHesderBinaryData(AliceModelData* model_, FILE* fp_);

	static bool SPReadNodesBinaryData(AliceModelData* model_, FILE* fp_);

	static bool SPReadMeshBinaryData(AliceModelData* model_, FILE* fp_);

#pragma endregion

#pragma endregion

#pragma region アニメーションデータ

#pragma region アスキー

	static void SPReadMotionData(std::stringstream& data_, std::string& str_, MotionData* motionData_);

	static void SPReadMotionHesderData(std::stringstream& data_, std::string& str_, MotionData* motion_);

	static void SPReadMotionNodesData(std::stringstream& data_, std::string& str_, MotionData* motion_);
	static void SPReadMotionNodeData(std::stringstream& data_, std::string& str_, MotionNode& node_);

#pragma endregion

#pragma region バイナリ

	static bool SPReadMotionNodeBinaryData(MotionNode* node_, FILE* fp_);

#pragma endregion

#pragma endregion

#pragma region その他

	static void SPReadString(const std::string& strData_, std::string& str_);
	static void SPReadFlag(const std::string& strFlag_, bool& flag_);
	static void SPReadNumber(const std::string& strNum_, float& num_);
	static void SPReadNumber(const std::string& strNum_, uint32_t& num_);
	static void SPReadVertex(const std::string& strVertex_, PosNormUvTangeColSkin& vertex_);
	static void SPReadVector3(const std::string& strVec_, AliceMathF::Vector3& vec_);
	static void SPReadVector3(const std::string& strVec_, AliceMathF::Vector4& vec_);
	static void SPReadVector2(const std::string& strVec_, AliceMathF::Vector2& vec_);
	static void SPReadVector4(const std::string& strVec_, AliceMathF::Vector4& vec_);
	static void SPReadQuaternion(const std::string& strVec_, AliceMathF::Quaternion& quaternion_);
	static void SPReadMatrix(const std::string& strMat_, AliceMathF::Matrix4& mat_);
	static float SPAtof(const std::string& str_);
	static void SPReadVectorKey(const std::string& strVec_, VectorKey& vectorKey_);
	static void SPReadQuaternionKey(const std::string& strVec_, QuaternionKey& quaternionKey_);

#pragma endregion

};