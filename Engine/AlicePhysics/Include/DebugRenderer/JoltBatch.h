#pragma once

#include<JoltDebugRenderPrimitive.h>

namespace AlicePhysics
{
#if defined(_DEBUG) && defined(JPH_DEBUG_RENDERER)

	class JoltBatch : public JPH::RefTargetVirtual,public JoltDebugRenderPrimitive
	{
	public:

		JoltBatch(ICommandList* commandList_,D3D_PRIMITIVE_TOPOLOGY type_);

		virtual void AddRef() override {
			JoltDebugRenderPrimitive::AddRef();
		}
		virtual void Release() override {
			if ( --mRefCount == 0 ) delete this;
		}

	private:

		//コピーコンストラクタ・代入演算子削除
		JoltBatch& operator=(const JoltBatch&) = delete;
		JoltBatch(const JoltBatch&) = delete;
	};
#endif // _DEBUG
}