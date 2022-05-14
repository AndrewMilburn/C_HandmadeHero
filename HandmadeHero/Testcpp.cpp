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

	int sizeOfChar = sizeof(char unsigned);
	int sizeOfInt = sizeof(int);
	int sizeOfShort = sizeof(short);
	int sizeOfProjectile = sizeof(Projectile);

	
	test.isThisOnFire = 1;
	test.damage = 2;
	test.particlesPerSecond = 3;
	test.howManyCooks = 4;

	unsigned short* wrongPointer = (unsigned short*)& test;

	Projectile projectiles[40];

	int whichProjectile = 4;
	projectiles[whichProjectile];

	Projectile* projectilePointer = projectiles;

	int sizeOfProjectiles = sizeof(projectiles);	// 16 * 40?
	int sizeOfProjectilePointer = sizeof(projectilePointer);
	short shortSizeOfPP = sizeof(projectilePointer);

	projectiles[30].damage = 60;
	//(projectilePointer + 30 * sizeof(Projectile))->damage = 100;
	(projectilePointer + 30)->damage = 100;
	((Projectile*)((char*)projectilePointer + 30 * sizeof(Projectile)))->damage = 120;

	int x = 5;
	int y = 10;
	int z = 12;

	x = (1 << 4) | (1 << 8);
	y = (1 << 4) | (1 << 14);
	x = x ^ y;
	x = x ^ y;


	integer = 5, integer = 5 + 2, integer = integer + 7;


	Foo();
}

