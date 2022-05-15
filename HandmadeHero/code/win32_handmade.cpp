#include <Windows.h>

int CALLBACK
WinMain(HINSTANCE hInstance,
        HINSTANCE hPrevInstance,
        PSTR lpCmdLine,
        INT nCmdShow)
{
    MessageBoxA(0, "This is Handmade Hero", "Handmade",
        MB_OK | MB_ICONEXCLAMATION);
    
    return 0;
}