#pragma once

#include<Defined.h>

ALICE_SUPPRESS_WARNINGS_BEGIN

#if _DEBUG && defined(JPH_DEBUG_RENDERER)
#include <Jolt/Jolt.h>
#include <Jolt/Renderer/DebugRenderer.h>
#endif // _DEBUG


ALICE_SUPPRESS_WARNINGS_END

#include<VertexBuffer.h>
#include<IndexBuffer.h>
#include<ConstantBuffer.h>
#include<CommandList.h>

#if defined(_DEBUG) && defined(JPH_DEBUG_RENDERER)
namespace AlicePhysics
{
	class JoltDebugRenderPrimitive : public JPH::RefTarget<JoltDebugRenderPrimitive>
	{
	private:
		int8_t PADING[ 4 ];

	protected:

		ICommandList* commandList = nullptr;

		std::unique_ptr<IVertexBuffer>vertexBuffer;
		std::unique_ptr<IIndexBuffer>indexBuffer;
		std::unique_ptr<IConstantBuffer>constantBuffer;

		size_t indexDrawCount = 0;
		size_t vertexDrawCount = 0;

		D3D_PRIMITIVE_TOPOLOGY type;

	private:
		int8_t PADING2[ 4 ];

	public:
		JoltDebugRenderPrimitive(ICommandList* commandList_,D3D_PRIMITIVE_TOPOLOGY type_);

		void CreateVertexBuffer(size_t numVtx_,size_t vtxSize_,const void* data_ = nullptr);
		void CreateIndexBuffer(size_t numIdx_,const uint32_t* data_ = nullptr);
		void CreateConstantBuffer(size_t vtxSize_, void* data_ = nullptr);

		void VertexUpdate(void* data_,size_t length_);
		void IndexUpdate(uint32_t* data_,size_t length_);
		void ConstantUpdate(void* data_);
		void Draw();
		void GeometryDraw();
		void GeometryOFFDraw();

	private:
		//コピーコンストラクタ・代入演算子削除
		JoltDebugRenderPrimitive& operator=(const JoltDebugRenderPrimitive&) = delete;
		JoltDebugRenderPrimitive(const JoltDebugRenderPrimitive&) = delete;
	};

}
#endif // _DEBUG

