// win32_handmade.cpp

#include <windows.h>

LRESULT CALLBACK WindowProc(HWND window, UINT message,
                            WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
        case WM_SIZE:
        {
            OutputDebugString( "WM_SIZE Detected\n" );
            break;
        }
        case WM_DESTROY:
        {
            OutputDebugString( "WM_DESTROY Detected\n" );
            break;
        }
        case WM_CLOSE:
        {
            OutputDebugString( "WM_CLOSE Detected\n" );
            break;
        }
        case WM_ACTIVATEAPP:
        {
            OutputDebugString( "WM_ACTIVATEAPP Detected\n" );
            break;
        }
        default:
        {
            //OutputDebugString( "Default Case Detected\n" );
            break;
        }
    }

        

}


int  CALLBACK  WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance,
                        LPSTR lpCmdLine, int nShowCmd)
{
    //MessageBox(0, "Hello Handmade Hero",
    //           "HMH #001", MB_OK | MB_ICONINFORMATION);

    WNDCLASS WindowClass = {};
        //TODO: Check whether the below flags are needed
        WindowClass.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
        WindowClass.lpfnWndProc = ;
        WindowClass.hInstance = hInstance;
        //WindowClass.hIcon;
        WindowClass.lpszClassName="Handmade Hero WindowClass";

    return(0);
}

