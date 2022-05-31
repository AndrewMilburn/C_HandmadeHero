// win32_handmade.cpp

#include <windows.h>

int  CALLBACK  WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance,
                        LPSTR lpCmdLine, int nShowCmd){
    
    MessageBox(0, "Hello Handmade Hero",
               "HMH #001", MB_OK | MB_ICONINFORMATION);
    
    return(0);
}
