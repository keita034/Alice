#pragma once

#include<JoltDebugRenderPrimitive.h>

namespace AlicePhysics
{
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
	};

}