#include "JoltBatch.h"

#ifdef _DEBUG
AlicePhysics::JoltBatch::JoltBatch(ICommandList* commandList_,D3D_PRIMITIVE_TOPOLOGY type_)
	: AlicePhysics::JoltDebugRenderPrimitive(commandList_,type_){}
#endif // _DEBUG

