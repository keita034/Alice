#include"AliceFramework.h"
#include"GameMain.h"

#ifdef _DEBUG

#include<dxgidebug.h>

struct D3DResouceLeakChecker
{
	~D3DResouceLeakChecker()
	{
		// リソースリークチェック
		Microsoft::WRL::ComPtr<IDXGIDebug1>debug;
		if ( SUCCEEDED(DXGIGetDebugInterface1(0,IID_PPV_ARGS(&debug))) )
		{
			debug->ReportLiveObjects(DXGI_DEBUG_ALL,DXGI_DEBUG_RLO_ALL);
			debug->ReportLiveObjects(DXGI_DEBUG_APP,DXGI_DEBUG_RLO_ALL);
			debug->ReportLiveObjects(DXGI_DEBUG_D3D12,DXGI_DEBUG_RLO_ALL);
		}
}

};

int main()
{
	D3DResouceLeakChecker checker;
	//AliceFramework::SDebugInitialize();

#else
int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int)
{
#endif

	std::unique_ptr<AliceFramework>lGame = std::make_unique<GameMain>();

	lGame->Run();

	return 0;
}
