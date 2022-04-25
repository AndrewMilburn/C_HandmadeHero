// test.cpp

#include <windows.h>

/*
Looooooong comment
*/

// Function declaration
void Foo(void);  // Function signature or prototype



int  CALLBACK  main
(
    HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPSTR     lpCmdLine,
    int       nShowCmd
)
{
    Foo();
}

// Function definition
void Foo(void)
{
    OutputDebugStringA("This is the first thing debugged\n");
}