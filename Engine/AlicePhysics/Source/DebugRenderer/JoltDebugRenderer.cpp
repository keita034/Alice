
#include "JoltDebugRenderer.h"
#include <RootSignature.h>
#include<AliceAssert.h>

#include<JoltBoxShape.h>
#include<JoltMeshShape.h>
#include<JoltCapsuleShape.h>
#include<JoltSphereShape.h>

#if _DEBUG && defined(JPH_DEBUG_RENDERER)


void AlicePhysics::JoltDebugRenderer::Initialize()
{
	DebugRenderer::Initialize();

	blendDesc.RenderTarget[ 0 ].LogicOpEnable = false;
	blendDesc.RenderTarget[ 0 ].LogicOp = D3D12_LOGIC_OP_NOOP;
	blendDesc.RenderTarget[ 0 ].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
	blendDesc.RenderTarget[ 0 ].BlendEnable = true;
	blendDesc.RenderTarget[ 0 ].SrcBlend = D3D12_BLEND_SRC_ALPHA;
	blendDesc.RenderTarget[ 0 ].DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
	blendDesc.RenderTarget[ 0 ].BlendOp = D3D12_BLEND_OP_ADD;
	blendDesc.RenderTarget[ 0 ].SrcBlendAlpha = D3D12_BLEND_ZERO;
	blendDesc.RenderTarget[ 0 ].DestBlendAlpha = D3D12_BLEND_ZERO;
	blendDesc.RenderTarget[ 0 ].BlendOpAlpha = D3D12_BLEND_OP_ADD;

	linePrimitive = std::make_unique<JoltDebugRenderPrimitive>(commandList,D3D_PRIMITIVE_TOPOLOGY_LINELIST);
	linePrimitive->CreateVertexBuffer(2 * MAX_LINES,sizeof(Point));

	trianglePrimitive = std::make_unique<JoltDebugRenderPrimitive>(commandList,D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	trianglePrimitive->CreateVertexBuffer(3 * MAX_TRIANGLES,sizeof(Point));

	lightConstantBuffer = CreateUniqueConstantBuffer(sizeof(ConstBufferLightData),AdaptersIndex::MAIN);

	PCreateLinePipelineState();
	PCreateTrianglePipelineState();

	//ピクセルシェーダの読み込み
	std::unique_ptr<IShader>lPixelShader = CreateUniqueShader("Resources/Shaders/AlicePhysics/Geometry/JoltDebugRendererGeometryPS.hlsl","main","ps_5_0",IShader::ShaderType::PS);
	//頂点シェーダの読み込み
	std::unique_ptr<IShader>lVertexShader = CreateUniqueShader("Resources/Shaders/AlicePhysics/Geometry/JoltDebugRendererGeometryVS.hlsl");

	PCreateGeometryWireframeipelineState(lPixelShader.get(),lVertexShader.get());
	PCreateGeometryFFPipelineState(lPixelShader.get(),lVertexShader.get());
	PCreateGeometryFBPipelineState(lPixelShader.get(),lVertexShader.get());
	PCreateGeometryOFFPipelineState(lPixelShader.get(),lVertexShader.get());

	JoltBoxShape::SetRenderer(this);
	JoltMeshShape::SetRenderer(this);
	JoltCapsuleShape::SetRenderer(this);
	JoltSphereShape::SetRenderer(this);

}

AlicePhysics::JoltDebugRenderer::JoltDebugRenderer()
{

}

void AlicePhysics::JoltDebugRenderer::DrawLine(JPH::RVec3Arg inFrom,JPH::RVec3Arg inTo,JPH::ColorArg inColor)
{
	Line lLine;

	lLine.from = { inFrom ,inColor.ToVec4() };
	lLine.to = { inTo ,inColor.ToVec4() };

	lines.emplace_back(lLine);
}

void AlicePhysics::JoltDebugRenderer::DrawTriangle(JPH::RVec3Arg inV1,JPH::RVec3Arg inV2,JPH::RVec3Arg inV3,JPH::ColorArg inColor)
{
	TriangleElement lTriangle;
	lTriangle.v1 = { inV1 ,inColor.ToVec4() };
	lTriangle.v2 = { inV2 ,inColor.ToVec4() };
	lTriangle.v3 = { inV3 ,inColor.ToVec4() };

	triangles.emplace_back(lTriangle);
}

JPH::DebugRenderer::Batch AlicePhysics::JoltDebugRenderer::CreateTriangleBatch(const Triangle* inTriangles,int inTriangleCount)
{
	if ( inTriangles == nullptr || inTriangleCount == 0 )
	{
		return emptyBatch;
	}

	JoltBatch* lPrimitive = new JoltBatch(commandList,D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	lPrimitive->CreateVertexBuffer(3 * static_cast< size_t >( inTriangleCount ),sizeof(Vertex),inTriangles);
	lPrimitive->CreateConstantBuffer(sizeof(ConstBufferData));

	return lPrimitive;
}

JPH::DebugRenderer::Batch  AlicePhysics::JoltDebugRenderer::CreateTriangleBatch(const Vertex* inVertices,int inVertexCount,const JPH::uint32* inIndices,int inIndexCount)
{
	if ( inVertices == nullptr || inVertexCount == 0 || inIndices == nullptr || inIndexCount == 0 )
	{
		return emptyBatch;
	}

	JoltBatch* lPrimitive = new JoltBatch(commandList,D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	lPrimitive->CreateVertexBuffer(static_cast< size_t > ( inVertexCount ),sizeof(Vertex),inVertices);
	lPrimitive->CreateIndexBuffer(static_cast< size_t > ( inIndexCount ),inIndices);
	lPrimitive->CreateConstantBuffer(sizeof(ConstBufferData));

	return lPrimitive;
}

void AlicePhysics::JoltDebugRenderer::DrawGeometry(JPH::RMat44Arg inModelMatrix,const JPH::AABox& inWorldSpaceBounds,float inLODScaleSq,JPH::ColorArg inModelColor,const GeometryRef& inGeometry,ECullMode inCullMode,ECastShadow inCastShadow,EDrawMode inDrawMode)
{
	ID3D12GraphicsCommandList* lCommandList = commandList->GetGraphicCommandList();

	static_cast< void >( inWorldSpaceBounds );

	AliceMathF::Matrix4 lMat = inModelMatrix;
	AliceMathF::Matrix4 lMat2 = lMat * viewMat * projectionMat;

	ConstBufferData lConstBufferData;
	lConstBufferData.isShadow = inCastShadow == ECastShadow::On ? 1.0f : 0.0f;
	lConstBufferData.color = inModelColor.ToVec4();
	lConstBufferData.world = lMat;
	lConstBufferData.matWorld = lMat2;

	if ( inDrawMode == EDrawMode::Wireframe )
	{
		lCommandList->SetGraphicsRootSignature(geometryWireframeRootSignature->GetRootSignature());
		lCommandList->SetPipelineState(geometryWireframePipelineState->GetPipelineState());
	}
	else
	{
		switch ( inCullMode )
		{
		case JPH::DebugRenderer::ECullMode::CullBackFace:
			lCommandList->SetGraphicsRootSignature(geometryFBRootSignature->GetRootSignature());
			lCommandList->SetPipelineState(geometryFBPipelineState->GetPipelineState());
			break;
		case JPH::DebugRenderer::ECullMode::CullFrontFace:
			lCommandList->SetGraphicsRootSignature(geometryFFRootSignature->GetRootSignature());
			lCommandList->SetPipelineState(geometryFFPipelineState->GetPipelineState());
			break;
		case JPH::DebugRenderer::ECullMode::Off:
			lCommandList->SetGraphicsRootSignature(geometryOFFRootSignature->GetRootSignature());
			lCommandList->SetPipelineState(geometryOFFPipelineState->GetPipelineState());
			break;
		default:
			AliceAssert(0,"inCullModeに良く分からん値が入っとるよ");
			break;
		}
	}

	lightConstantBuffer->Update(&lightData);

	for ( LOD& lod : inGeometry->mLODs )
	{
		if ( lod.mDistance > inLODScaleSq )
		{
			JoltBatch* lPrimitive = static_cast< JoltBatch* > ( lod.mTriangleBatch.GetPtr() );

			lCommandList->SetGraphicsRootConstantBufferView(1,lightConstantBuffer->GetAddress());

			if ( constantBufferPtr )
			{
				std::vector<std::unique_ptr<IConstantBuffer>>&lConstantBuffers = *constantBufferPtr;

				lConstantBuffers[index]->Update(&lConstBufferData);
				lCommandList->SetGraphicsRootConstantBufferView(0,lConstantBuffers[ index ]->GetAddress());
				lPrimitive->GeometryOFFDraw();
			}
			else
			{
				lPrimitive->ConstantUpdate(&lConstBufferData);

				lPrimitive->GeometryDraw();
			}

			index++;

			break;
		}
	}
}

void AlicePhysics::JoltDebugRenderer::DrawText3D(JPH::RVec3Arg inPosition,const JPH::string_view& inString,JPH::ColorArg inColor,float inHeight)
{
	static_cast< void >( inPosition );
	static_cast< void >( inString );
	static_cast< void >( inColor );
	static_cast< void >( inHeight );
}

void AlicePhysics::JoltDebugRenderer::Draw()
{
	ID3D12GraphicsCommandList* lCommandList = commandList->GetGraphicCommandList();

	LineDraw(lCommandList);
	TriangleDraw(lCommandList);
}

void AlicePhysics::JoltDebugRenderer::SetViewProjection(AliceMathF::Matrix4* viewMat_,AliceMathF::Matrix4* projectionMat_)
{
	if ( viewMat_ )
	{
		viewMat = *viewMat_;

	}
	if ( projectionMat_ )
	{
		projectionMat = *projectionMat_;

	}
}

void AlicePhysics::JoltDebugRenderer::SetLight(AliceMathF::Vector3* lightV_,AliceMathF::Vector4* lightColor_)
{
	if ( lightV_ )
	{
		lightData.vector = lightV_->Normal();

	}
	if ( lightColor_ )
	{
		lightData.color = *lightColor_;

	}
}

void AlicePhysics::JoltDebugRenderer::SetDevice(IDevice* device_)
{
	device = device_;
}

void AlicePhysics::JoltDebugRenderer::SetCommandList(ICommandList* commandList_)
{
	commandList = commandList_;
}

void AlicePhysics::JoltDebugRenderer::SetConstant(std::vector<std::unique_ptr<IConstantBuffer>>* buff_)
{
	constantBufferPtr = buff_;
	index = 0;
}

void AlicePhysics::JoltDebugRenderer::LineDraw(ID3D12GraphicsCommandList* commandList_)
{
	commandList_->SetPipelineState(linePipelineState->GetPipelineState());
	commandList_->SetGraphicsRootSignature(lineRootSignature->GetRootSignature());

	linePrimitive->VertexUpdate(lines.data(),lines.size());
	linePrimitive->Draw();

	lines.clear();
}

void AlicePhysics::JoltDebugRenderer::TriangleDraw(ID3D12GraphicsCommandList* commandList_)
{
	commandList_->SetPipelineState(trianglePipelineState->GetPipelineState());
	commandList_->SetGraphicsRootSignature(triangleRootSignature->GetRootSignature());

	trianglePrimitive->VertexUpdate(triangles.data(),triangles.size());
	trianglePrimitive->Draw();

	triangles.clear();

}

void AlicePhysics::JoltDebugRenderer::PCreateLinePipelineState()
{
	linePipelineState = std::make_unique<PipelineState>();

	D3D12_INPUT_ELEMENT_DESC lLinedesc[ ] =
	{
		//座標
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
		//カラー 
		{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
	};

	linePipelineState->SetInputLayout(lLinedesc,2);

	//頂点シェーダの読み込み
	std::unique_ptr<IShader>lVertexShader = CreateUniqueShader("Resources/Shaders/AlicePhysics/Line/JoltDebugRendererLineVS.hlsl");
	linePipelineState->SetVertexShader(lVertexShader->GetShader());

	//ピクセルシェーダの読み込み
	std::unique_ptr<IShader>lPixelShader = CreateUniqueShader("Resources/Shaders/AlicePhysics/Line/JoltDebugRendererLinePS.hlsl","main","ps_5_0",IShader::ShaderType::PS);
	linePipelineState->SetPixelShader(lPixelShader->GetShader());

	linePipelineState->SetPrimitiveType(D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE);

	linePipelineState->SetBlend(blendDesc);

	lineRootSignature = CreateUniqueRootSignature();
	lineRootSignature->Create(device->Get());

	linePipelineState->SetRootSignature(lineRootSignature->GetRootSignature());

	linePipelineState->Create();
}

void AlicePhysics::JoltDebugRenderer::PCreateTrianglePipelineState()
{
	trianglePipelineState = std::make_unique<PipelineState>();

	D3D12_INPUT_ELEMENT_DESC lLinedesc[ ] =
	{
		//座標
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
		//カラー 
		{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
	};

	trianglePipelineState->SetInputLayout(lLinedesc,2);

	//頂点シェーダの読み込み
	std::unique_ptr<IShader>lVertexShader = CreateUniqueShader("Resources/Shaders/AlicePhysics/Triangle/JoltDebugRendererTriangleVS.hlsl");
	trianglePipelineState->SetVertexShader(lVertexShader->GetShader());

	//ピクセルシェーダの読み込み
	std::unique_ptr<IShader>lPixelShader = CreateUniqueShader("Resources/Shaders/AlicePhysics/Triangle/JoltDebugRendererTrianglePS.hlsl","main","ps_5_0",IShader::ShaderType::PS);
	trianglePipelineState->SetPixelShader(lPixelShader->GetShader());

	trianglePipelineState->SetPrimitiveType(D3D12_PRIMITIVE_TOPOLOGY_TYPE::D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE);

	trianglePipelineState->SetBlend(blendDesc);

	triangleRootSignature = CreateUniqueRootSignature();
	triangleRootSignature->Create(device->Get());

	trianglePipelineState->SetRootSignature(triangleRootSignature->GetRootSignature());

	trianglePipelineState->Create();
}

void AlicePhysics::JoltDebugRenderer::PCreateGeometryWireframeipelineState(IShader* ps_,IShader* vs_)
{
	geometryWireframePipelineState = std::make_unique<PipelineState>();

	D3D12_INPUT_ELEMENT_DESC lLinedesc[ ] =
	{
		{ "POSITION",	0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT,	0,	D3D12_APPEND_ALIGNED_ELEMENT,	D3D12_INPUT_CLASSIFICATION::D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,	0}, // float3のPOSITION
		{ "NORMAL"	,	0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT,	0,	D3D12_APPEND_ALIGNED_ELEMENT,	D3D12_INPUT_CLASSIFICATION::D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,	0}, // float3のNORMAL
		{ "TEXCOORD",	0, DXGI_FORMAT::DXGI_FORMAT_R32G32_FLOAT,		0,	D3D12_APPEND_ALIGNED_ELEMENT,	D3D12_INPUT_CLASSIFICATION::D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,	0}, // float2のTEXCOORD
		{ "COLOR"	,	0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT,	0,	D3D12_APPEND_ALIGNED_ELEMENT,	D3D12_INPUT_CLASSIFICATION::D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,	0},

	};

	geometryWireframePipelineState->SetInputLayout(lLinedesc,_countof(lLinedesc));

	geometryWireframePipelineState->SetVertexShader(vs_->GetShader());
	geometryWireframePipelineState->SetPixelShader(ps_->GetShader());

	geometryWireframePipelineState->SetPrimitiveType(D3D12_PRIMITIVE_TOPOLOGY_TYPE::D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE);

	geometryWireframePipelineState->SetBlend(blendDesc);
	geometryWireframePipelineState->SetCullMode(D3D12_CULL_MODE::D3D12_CULL_MODE_BACK);
	geometryWireframePipelineState->SetFillMode(D3D12_FILL_MODE::D3D12_FILL_MODE_WIREFRAME);

	geometryWireframeRootSignature = CreateUniqueRootSignature();
	geometryWireframeRootSignature->Add(IRootSignature::RootType::CBV,0);
	geometryWireframeRootSignature->Add(IRootSignature::RootType::CBV,1);
	geometryWireframeRootSignature->Create(device->Get());

	geometryWireframePipelineState->SetRootSignature(geometryWireframeRootSignature->GetRootSignature());

	geometryWireframePipelineState->Create();
}

void AlicePhysics::JoltDebugRenderer::PCreateGeometryFFPipelineState(IShader* ps_,IShader* vs_)
{
	geometryFFPipelineState = std::make_unique<PipelineState>();

	D3D12_INPUT_ELEMENT_DESC lLinedesc[ ] =
	{
		{ "POSITION",	0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT,	0,	D3D12_APPEND_ALIGNED_ELEMENT,	D3D12_INPUT_CLASSIFICATION::D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,	0}, // float3のPOSITION
		{ "NORMAL"	,	0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT,	0,	D3D12_APPEND_ALIGNED_ELEMENT,	D3D12_INPUT_CLASSIFICATION::D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,	0}, // float3のNORMAL
		{ "TEXCOORD",	0, DXGI_FORMAT::DXGI_FORMAT_R32G32_FLOAT,		0,	D3D12_APPEND_ALIGNED_ELEMENT,	D3D12_INPUT_CLASSIFICATION::D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,	0}, // float2のTEXCOORD
		{ "COLOR"	,	0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT,	0,	D3D12_APPEND_ALIGNED_ELEMENT,	D3D12_INPUT_CLASSIFICATION::D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,	0},

	};

	geometryFFPipelineState->SetInputLayout(lLinedesc,_countof(lLinedesc));

	geometryFFPipelineState->SetVertexShader(vs_->GetShader());
	geometryFFPipelineState->SetPixelShader(ps_->GetShader());

	geometryFFPipelineState->SetPrimitiveType(D3D12_PRIMITIVE_TOPOLOGY_TYPE::D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE);

	geometryFFPipelineState->SetBlend(blendDesc);
	geometryFFPipelineState->SetCullMode(D3D12_CULL_MODE::D3D12_CULL_MODE_FRONT);

	geometryFFRootSignature = CreateUniqueRootSignature();
	geometryFFRootSignature->Add(IRootSignature::RootType::CBV,0);
	geometryFFRootSignature->Add(IRootSignature::RootType::CBV,1);
	geometryFFRootSignature->Create(device->Get());

	geometryFFPipelineState->SetRootSignature(geometryFFRootSignature->GetRootSignature());

	geometryFFPipelineState->Create();
}

void AlicePhysics::JoltDebugRenderer::PCreateGeometryFBPipelineState(IShader* ps_,IShader* vs_)
{
	geometryFBPipelineState = std::make_unique<PipelineState>();

	D3D12_INPUT_ELEMENT_DESC lLinedesc[ ] =
	{
		{ "POSITION",	0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT,	0,	D3D12_APPEND_ALIGNED_ELEMENT,	D3D12_INPUT_CLASSIFICATION::D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,	0}, // float3のPOSITION
		{ "NORMAL"	,	0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT,	0,	D3D12_APPEND_ALIGNED_ELEMENT,	D3D12_INPUT_CLASSIFICATION::D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,	0}, // float3のNORMAL
		{ "TEXCOORD",	0, DXGI_FORMAT::DXGI_FORMAT_R32G32_FLOAT,		0,	D3D12_APPEND_ALIGNED_ELEMENT,	D3D12_INPUT_CLASSIFICATION::D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,	0}, // float2のTEXCOORD
		{ "COLOR"	,	0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT,	0,	D3D12_APPEND_ALIGNED_ELEMENT,	D3D12_INPUT_CLASSIFICATION::D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,	0},

	};

	geometryFBPipelineState->SetInputLayout(lLinedesc,_countof(lLinedesc));

	geometryFBPipelineState->SetVertexShader(vs_->GetShader());
	geometryFBPipelineState->SetPixelShader(ps_->GetShader());

	geometryFBPipelineState->SetPrimitiveType(D3D12_PRIMITIVE_TOPOLOGY_TYPE::D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE);

	geometryFBPipelineState->SetBlend(blendDesc);
	geometryFBPipelineState->SetCullMode(D3D12_CULL_MODE::D3D12_CULL_MODE_BACK);

	geometryFBRootSignature = CreateUniqueRootSignature();
	geometryFBRootSignature->Add(IRootSignature::RootType::CBV,0);
	geometryFBRootSignature->Add(IRootSignature::RootType::CBV,1);
	geometryFBRootSignature->Create(device->Get());

	geometryFBPipelineState->SetRootSignature(geometryFBRootSignature->GetRootSignature());

	geometryFBPipelineState->Create();
}

void AlicePhysics::JoltDebugRenderer::PCreateGeometryOFFPipelineState(IShader* ps_,IShader* vs_)
{
	geometryOFFPipelineState = std::make_unique<PipelineState>();

	D3D12_INPUT_ELEMENT_DESC lLinedesc[ ] =
	{
		{ "POSITION",	0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT,	0,	D3D12_APPEND_ALIGNED_ELEMENT,	D3D12_INPUT_CLASSIFICATION::D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,	0}, // float3のPOSITION
		{ "NORMAL"	,	0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT,	0,	D3D12_APPEND_ALIGNED_ELEMENT,	D3D12_INPUT_CLASSIFICATION::D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,	0}, // float3のNORMAL
		{ "TEXCOORD",	0, DXGI_FORMAT::DXGI_FORMAT_R32G32_FLOAT,		0,	D3D12_APPEND_ALIGNED_ELEMENT,	D3D12_INPUT_CLASSIFICATION::D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,	0}, // float2のTEXCOORD
		{ "COLOR"	,	0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT,	0,	D3D12_APPEND_ALIGNED_ELEMENT,	D3D12_INPUT_CLASSIFICATION::D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,	0},

	};

	geometryOFFPipelineState->SetInputLayout(lLinedesc,_countof(lLinedesc));

	geometryOFFPipelineState->SetVertexShader(vs_->GetShader());
	geometryOFFPipelineState->SetPixelShader(ps_->GetShader());

	geometryOFFPipelineState->SetPrimitiveType(D3D12_PRIMITIVE_TOPOLOGY_TYPE::D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE);

	geometryOFFPipelineState->SetBlend(blendDesc);
	geometryOFFPipelineState->SetCullMode(D3D12_CULL_MODE::D3D12_CULL_MODE_NONE);

	geometryOFFRootSignature = CreateUniqueRootSignature();
	geometryOFFRootSignature->Add(IRootSignature::RootType::CBV,0);
	geometryOFFRootSignature->Add(IRootSignature::RootType::CBV,1);
	geometryOFFRootSignature->Create(device->Get());

	geometryOFFPipelineState->SetRootSignature(geometryOFFRootSignature->GetRootSignature());

	geometryOFFPipelineState->Create();
}
#endif // _DEBUG