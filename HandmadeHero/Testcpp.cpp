// Testcpp.cpp

#include <Windows.h>

void Foo(void)

{
	const char* foo1 = "Debug Print After WinMain\n";
	OutputDebugStringA(foo1);
}

struct Projectile
{
	char unsigned isThisOnFire;	// 1 if it is, 0 if not
	int damage;
	int particlesPerSecond;	// Any vfx
	short howManyCooks;	// Can't have too many!
};

int CALLBACK WinMain
(	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	INT nCmdShow
)
{
	int integer;
	/*
	// 8 bits = 255 values
	char smallS;
	char  unsigned smallU;
	
	// 16 bits = 65536 values
	short mediumS;
	short unsigned mediumU;

	// 32 bits ~ 4 billion
	int largeS;
	int   unsigned largeU;
	*/
	
	Projectile test;

	test.isThisOnFire = 1;
	test.damage = 123456;
	test.particlesPerSecond = 1234567;
	test.howManyCooks = 24;

	integer = 5;
	integer = 5 + 2;
	integer = integer + 7;


	Foo();
}

