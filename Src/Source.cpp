#include "CRenderWindow.h"
#include "CEngine.h"

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR lpCmdLine,
	_In_ int nCmdShow)
{

	CEngine Engine;
	if (!Engine.Init(hInstance, "Title", "MyWindowClass", 800, 600)) return 0;

	while (Engine.ProcessMessages())
	{
		Engine.Update();
		Engine.Render();
	}
	
	return 0;
}	