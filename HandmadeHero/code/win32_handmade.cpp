// win32_handmade.cpp

#include <windows.h>

#define localPersist static
#define globalVar static
#define internal static

// todo This is a global for now, but in future we'll call it from within the program
globalVar bool isRunning;

globalVar BITMAPINFO bitmapInfo;
globalVar void* bitmapMemory;
globalVar int bitmapWidth;
globalVar int bitmapHeight;



// Create the Back-Buffer
internal void Win32ResizeDIBSection( int width, int height )
{
    // todo - Bulletproof this
    // maybe don't free first - free after and if that fails free first

    if(bitmapMemory)
    {
        VirtualFree(bitmapMemory, 0, MEM_RELEASE);
    }

    bitmapWidth = width;
    bitmapHeight = height;

    bitmapInfo.bmiHeader.biSize = sizeof( bitmapInfo.bmiHeader );
    bitmapInfo.bmiHeader.biWidth = bitmapWidth;
    bitmapInfo.bmiHeader.biHeight = -bitmapHeight;
    bitmapInfo.bmiHeader.biPlanes = 1;
    bitmapInfo.bmiHeader.biBitCount = 32;
    bitmapInfo.bmiHeader.biCompression = BI_RGB;

    int bytesPerPixel = 4;
    int bitmapMemorySize = bytesPerPixel * (bitmapWidth * bitmapHeight);
    bitmapMemory = VirtualAlloc(0, bitmapMemorySize, MEM_COMMIT, PAGE_READWRITE);

}

// Write From the Back-Buffer
internal void Win32UpdateWindow(HDC backBuffer, RECT *windowRect, int x, int y, int width, int height )
{
    int windowWidth = windowRect->right - windowRect->left;
    int windowHeight = windowRect->bottom - windowRect->top;
    StretchDIBits(backBuffer,
        /*
                  x, y, width, height,
                  x, y, width, height,
        */          
                  0, 0, windowWidth, windowHeight,
                  0, 0, bitmapWidth, bitmapHeight,
                  bitmapMemory, &bitmapInfo,
                  DIB_RGB_COLORS, SRCCOPY );
}

LRESULT CALLBACK Win32MainWindowCallback(HWND window, UINT message,
                                        WPARAM wParam, LPARAM lParam)
{
    LRESULT result = 0;

    switch (message)
    {
        case WM_SIZE:
        {
            RECT clientRect;
            GetClientRect(window, &clientRect);
            int drawWidth = clientRect.right - clientRect.left;
            int drawHeight = clientRect.bottom - clientRect.top;
            Win32ResizeDIBSection(drawWidth, drawHeight);
            OutputDebugString( "WM_SIZE Detected\n" );
            break;
        }

        case WM_CLOSE:
        {
            // todo Handle this with message to user / saving code etc.
            isRunning = false;
            OutputDebugString( "WM_CLOSE Detected\n" );
            break;
        }
        case WM_ACTIVATEAPP:
        {
            OutputDebugString( "WM_ACTIVATEAPP Detected\n" );
            break;
        }
        case WM_DESTROY:
        {
            // todo Handle this with message to user / saving code etc.
            isRunning = false;
            OutputDebugString( "WM_DESTROY Detected\n" );
            break;
        }
        case WM_PAINT:
        {
            PAINTSTRUCT paint;
            HDC deviceContext = BeginPaint(window, &paint);
            int paintX = paint.rcPaint.left;
            int paintY = paint.rcPaint.top;
            int paintWidth = paint.rcPaint.right - paintX;
            int paintHeight = paint.rcPaint.bottom - paintY;

            RECT clientRect;
            GetClientRect( window, &clientRect );

            Win32UpdateWindow(deviceContext, &clientRect, paintX, paintY, paintWidth, paintHeight );
            EndPaint(window, &paint);
            break;
        }
        default:
        {
            //OutputDebugString( "Default Case Detected\n" );
            result = DefWindowProc(window, message, wParam, lParam);
            break;
        }
    }
    return(result);
}


int  CALLBACK  WinMain( HINSTANCE instance, HINSTANCE prevInstance,
                        LPSTR cmdLine, int showCmd)
{
    //MessageBox(0, "Hello Handmade Hero",
    //           "HMH #001", MB_OK | MB_ICONINFORMATION);

    WNDCLASS WindowClass = {};
    WindowClass.lpfnWndProc = Win32MainWindowCallback;
    WindowClass.hInstance = instance;
    //WindowClass.hIcon;
    WindowClass.lpszClassName="Handmade Hero WindowClass";

    if(RegisterClass( &WindowClass ))
    {
        HWND windowHandle = CreateWindowEx
        (
            0, WindowClass.lpszClassName,
            "Handmade Hero", WS_OVERLAPPEDWINDOW | WS_VISIBLE,
            CW_USEDEFAULT, CW_USEDEFAULT,
            CW_USEDEFAULT, CW_USEDEFAULT,
            0, 0,
            instance, 0
        );

        if(windowHandle)
        {
            isRunning = true;
            while(isRunning)
            {
                MSG message;
                BOOL messageResult = GetMessage( &message, 0, 0, 0 );
                if(messageResult > 0)
                {
                    TranslateMessage(&message);
                    DispatchMessage( &message );
                }
                else
                {
                    break;
                }
            }
        }
        else
        {
            // todo Log fail?
        }
    }
    else
    {
        // todo Log fail?
    }
    return(0);
}
