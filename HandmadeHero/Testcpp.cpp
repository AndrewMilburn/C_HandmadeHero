// Testcpp.cpp

#include <Windows.h>

void Foo(void)

{
	const char* foo1 = "Debug Print After WinMain\n";
	OutputDebugStringA(foo1);
}


int CALLBACK WinMain
(	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	INT nCmdShow
)
{
	int integer;
	// 8 bits = 255 values
	char smallS;
	char  unsigned smallU;
	
	// 16 bits = 65536 values
	short mediumS;
	short unsigned mediumU;

	// 32 bits ~ 4 billion
	int largeS;
	int   unsigned largeU;
	
	char unsigned test;

	test = 255;
	test = test + 1;

	integer = 5;
	integer = 5 + 2;
	integer = integer + 7;


	Foo();
}

