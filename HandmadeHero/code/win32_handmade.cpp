// win32_handmade.cpp

#include <windows.h>

#define localPersist static
#define globalVar static
#define internal static

// todo This is a global for now, but in future we'll call it from within the program
globalVar bool isRunning;

// Create the Back-Buffer
internal void Win32ResizeDIBSection( int width, int height )
{
    BITMAPINFO bitmapInfo;
    bitmapInfo.bmiHeader.biSize = sizeof( bitmapInfo.bmiHeader );
    bitmapInfo.bmiHeader.biWidth = width;
    bitmapInfo.bmiHeader.biHeight = height;
    bitmapInfo.bmiHeader.biPlanes = 1;
    bitmapInfo.bmiHeader.biBitCount = 32;
    bitmapInfo.bmiHeader.biCompression = BI_RGB;
    bitmapInfo.bmiHeader.biSizeImage = 0;
    bitmapInfo.bmiHeader.biXPelsPerMeter = 0;
    bitmapInfo.bmiHeader.biYPelsPerMeter = 0;
    bitmapInfo.bmiHeader.biClrUsed = 0;
    bitmapInfo.bmiHeader.biClrImportant = 0;

    void* BitmapMemory;
    HBITMAP = CreateDIBSection(
                                HDC              hdc, &bitmapInfo,
                                DIB_RGB_COLORS, VOID * *ppvBits,
                                0, 0);
}

// Write to the Back-Buffer
internal void Win32UpdateWindow( HDC surface, int x, int y, int width, int height )
{
    StretchDIBits(surface, 
                  x, y, width, height,
                  x, y, width, height,
                  const VOID * lpBits, const BITMAPINFO * lpbmi,
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
        case WM_DESTROY:
        {
            // todo Handle this with message to user / saving code etc.
            isRunning = false;
            OutputDebugString( "WM_DESTROY Detected\n" );
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
        case WM_PAINT:
        {
            PAINTSTRUCT paint;
            HDC deviceContext = BeginPaint(window, &paint);
            int paintX = paint.rcPaint.left;
            int paintY = paint.rcPaint.top;
            int paintWidth = paint.rcPaint.right - paintX;
            int paintHeight = paint.rcPaint.bottom - paintY;
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


int  CALLBACK  WinMain( HINSTANCE instance, HINSTANCE hPrevInstance,
                        LPSTR lpCmdLine, int nShowCmd)
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
