#include "JoltBatch.h"

#if _DEBUG && defined(JPH_DEBUG_RENDERER)
AlicePhysics::JoltBatch::JoltBatch(ICommandList* commandList_,D3D_PRIMITIVE_TOPOLOGY type_)
	: AlicePhysics::JoltDebugRenderPrimitive(commandList_,type_){}
#endif // _DEBUG

