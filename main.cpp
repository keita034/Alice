#include"AliceFramework.h"
#include"GameMain.h"

#ifdef _DEBUG
int main()
{
	GameMain::DebugInitialize();

#else
int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int)
{
#endif

	std::unique_ptr<AliceFramework>game = std::make_unique<GameMain>();

	game->Run();

	return 0;
}
