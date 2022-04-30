// Testcpp.cpp

#include <Windows.h>

void Foo(void);


int CALLBACK WinMain
(	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	INT nCmdShow
)
{
	Foo();
}

void Foo(void)

{
	const char* foo = "Debug Print After WinMain\n";

	OutputDebugStringA(foo);
}