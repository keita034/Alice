#pragma once



#include<PipelineState.h>
#include<RootSignature.h>
#include<AliceMathUtility.h>
#include<Device.h>
#include<CommandList.h>

#if defined(_DEBUG) && defined(JPH_DEBUG_RENDERER)



#include<JoltBatch.h>
namespace AlicePhysics
{

	class JoltDebugRenderer : public JPH::DebugRenderer
	{
	private:

		struct Point
		{
			AliceMathF::Vector3 position;
			AliceMathF::Vector4 color;
		};

		struct Line
		{
			Point from;
			Point to;
		};

		struct TriangleElement
		{
			Point v1;
			Point v2;
			Point v3;
		};

		struct ConstBufferLightData
		{
			AliceMathF::Vector3 vector;
			AliceMathF::Vector4 color;
		};

	private:

		ICommandList* commandList = nullptr;
		IDevice* device = nullptr;
		std::vector<std::unique_ptr<IConstantBuffer>>* constantBufferPtr = nullptr;

		const size_t MAX_LINES = 10240;
		const size_t MAX_TRIANGLES = 10240;

		D3D12_BLEND_DESC blendDesc;

		std::unique_ptr<PipelineState> linePipelineState;
		std::unique_ptr<IRootSignature> lineRootSignature;
		
		std::unique_ptr<PipelineState> trianglePipelineState;
		std::unique_ptr<IRootSignature> triangleRootSignature;

		std::unique_ptr<PipelineState> geometryFFPipelineState;
		std::unique_ptr<IRootSignature> geometryFFRootSignature;

		std::unique_ptr<PipelineState> geometryFBPipelineState;
		std::unique_ptr<IRootSignature> geometryFBRootSignature;

		std::unique_ptr<PipelineState> geometryOFFPipelineState;
		std::unique_ptr<IRootSignature> geometryOFFRootSignature;

		std::unique_ptr<PipelineState> geometryWireframePipelineState;
		std::unique_ptr<IRootSignature> geometryWireframeRootSignature;

		std::unique_ptr<JoltDebugRenderPrimitive> linePrimitive;
		std::unique_ptr<JoltDebugRenderPrimitive> trianglePrimitive;

		std::unique_ptr<IConstantBuffer> lightConstantBuffer;

		std::vector<Line> lines;
		std::vector<TriangleElement> triangles;

		Batch emptyBatch;

		size_t index =0 ;

		AliceMathF::Matrix4 viewMat;
		AliceMathF::Matrix4 projectionMat;
		AliceMathF::Vector3 lightV;
		AliceMathF::Vector4 lightColor;

		ConstBufferLightData lightData;

	public:

		struct ConstBufferData
		{
			//ワールド行列
			AliceMathF::Matrix4 world;
			//ワールド座標
			AliceMathF::Matrix4 matWorld;
			//カラー
			AliceMathF::Vector4 color;
			//影があるか
			float isShadow = 0.0f;
		};

	public:

		JoltDebugRenderer();
		~JoltDebugRenderer() = default;

		void Initialize();

		void DrawLine(JPH::RVec3Arg inFrom,JPH::RVec3Arg inTo,JPH::ColorArg inColor) override;
		void DrawTriangle(JPH::RVec3Arg inV1,JPH::RVec3Arg inV2,JPH::RVec3Arg inV3,JPH::ColorArg inColor) override;
		JPH::DebugRenderer::Batch  CreateTriangleBatch(const Triangle* inTriangles,int inTriangleCount) override;
		JPH::DebugRenderer::Batch  CreateTriangleBatch(const Vertex* inVertices,int inVertexCount,const JPH::uint32* inIndices,int inIndexCount) override;
		void DrawGeometry(JPH::RMat44Arg inModelMatrix,const JPH::AABox& inWorldSpaceBounds,float inLODScaleSq,JPH::ColorArg inModelColor,const GeometryRef& inGeometry,ECullMode inCullMode,ECastShadow inCastShadow,EDrawMode inDrawMode) override;
		void DrawText3D(JPH::RVec3Arg inPosition,const JPH::string_view& inString,JPH::ColorArg inColor,float inHeight) override;

		void Draw();

		void SetViewProjection(const AliceMathF::Matrix4& viewMat_,const AliceMathF::Matrix4& projectionMat_);
		void SetLight(AliceMathF::Vector3* lightV_,AliceMathF::Vector4* lightColor_);
		void SetDevice(IDevice* device_);
		void SetCommandList(ICommandList* commandList_);
		void SetConstant(std::vector<std::unique_ptr<IConstantBuffer>>* buff_);

	private:

		void LineDraw(ID3D12GraphicsCommandList* commandList_);
		void TriangleDraw(ID3D12GraphicsCommandList* commandList_);

		void PCreateLinePipelineState();
		void PCreateTrianglePipelineState();
		void PCreateGeometryWireframeipelineState(IShader* ps_,IShader* vs_);
		void PCreateGeometryFFPipelineState(IShader* ps_,IShader* vs_);
		void PCreateGeometryFBPipelineState(IShader* ps_,IShader* vs_);
		void PCreateGeometryOFFPipelineState(IShader* ps_,IShader* vs_);

		//コピーコンストラクタ・代入演算子削除
		JoltDebugRenderer& operator=(const JoltDebugRenderer&) = delete;
		JoltDebugRenderer(const JoltDebugRenderer&) = delete;
	};
}

#endif // _DEBUG
