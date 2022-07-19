// win32_handmade.cpp

#include <windows.h>
#include <stdint.h>

#define localPersist static
#define globalVar static
#define internal static

typedef uint8_t uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;
typedef uint64_t uint64;

typedef int8_t int8;
typedef int16_t int16;
typedef int32_t int32;
typedef int64_t int64;

struct win32BackBuffer
{
    BITMAPINFO bitmapInfo;
    void* bitmapMemory;
    int bitmapWidth;
    int bitmapHeight;
    int bitmapPitch;
    int bytesPerPixel;
};

struct win32WindowDimension
{
    int dimX;
    int dimY;
};

// todo This is a global for now, but in future we'll call it from within the program
globalVar bool isRunning;
globalVar win32BackBuffer globalBackBuffer;

win32WindowDimension
Win32GetWindowDimension( HWND window )
{
    win32WindowDimension result;
    RECT clientRect;
    GetClientRect( window, &clientRect );
    result.dimX = clientRect.right - clientRect.left;
    result.dimY = clientRect.bottom - clientRect.top;

    return(result);
}


internal void
RenderWeirdGradient(win32BackBuffer buffer, int xOffset, int yOffset)
{
    uint8* row = (uint8*)buffer.bitmapMemory;

    for(int y = 0; y < buffer.bitmapHeight; y++)
    {
        uint32* pixel = (uint32*)row;
        for(int x = 0; x < buffer.bitmapWidth; x++)
        {
            /*
            Pixel in memory - RR GG BB xx ?
            Apparently not - Actually BB GG RR xx
            Then putting it in a 32-bit Register (little endian, member?)
            Becomes xx RR GG BB
            */

            uint8 blue = (x + xOffset);
            uint8 green = (y + yOffset);

            *pixel++ = ((green << 8) | blue);   // Shift Green into place
        }
        row += buffer.bitmapPitch;
    }

}
// Create the Back-Buffer
internal void
Win32ResizeDIBSection(win32BackBuffer *buffer, int width, int height )
{
    // todo - Bulletproof this
    // maybe don't free first - free after and if that fails free first

    if(buffer->bitmapMemory)
    {
        VirtualFree(buffer->bitmapMemory, 0, MEM_RELEASE);
    }

    buffer->bitmapWidth = width;
    buffer->bitmapHeight = height;
    buffer->bytesPerPixel = 4;
    buffer->bitmapInfo.bmiHeader.biSize = sizeof( buffer->bitmapInfo.bmiHeader );
    buffer->bitmapInfo.bmiHeader.biWidth = buffer->bitmapWidth;
    buffer->bitmapInfo.bmiHeader.biHeight = -buffer->bitmapHeight;
    buffer->bitmapInfo.bmiHeader.biPlanes = 1;
    buffer->bitmapInfo.bmiHeader.biBitCount = 32;
    buffer->bitmapInfo.bmiHeader.biCompression = BI_RGB;

    int bitmapMemorySize = buffer->bytesPerPixel * (buffer->bitmapWidth * buffer->bitmapHeight);
    buffer->bitmapMemory = VirtualAlloc(0, bitmapMemorySize, MEM_COMMIT, PAGE_READWRITE);
    // todo - Probably Clear this to Black later
    buffer->bitmapPitch = width * buffer->bytesPerPixel;
}

// Write From the Back-Buffer
internal void
Win32BlitToWindow(HDC deviceContext, int windowWidth, int windowHeight,
                  win32BackBuffer buffer, int x, int y, int width, int height )
{
    StretchDIBits(deviceContext,
        /*
                  x, y, width, height,
                  x, y, width, height,
        */          
                  0, 0, windowWidth, windowHeight,
                  0, 0, buffer.bitmapWidth, buffer.bitmapHeight,
                  buffer.bitmapMemory, &buffer.bitmapInfo,
                  DIB_RGB_COLORS, SRCCOPY );
}

LRESULT CALLBACK
Win32MainWindowCallback(HWND window, UINT message,
                        WPARAM wParam, LPARAM lParam)
{
    LRESULT result = 0;

    switch (message)
    {
        case WM_SIZE:
        {
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

            win32WindowDimension dim = Win32GetWindowDimension( window );
            Win32BlitToWindow(deviceContext, dim.dimX, dim.dimY, globalBackBuffer , paintX, paintY, paintWidth, paintHeight );
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


int  CALLBACK 
WinMain( HINSTANCE instance, HINSTANCE prevInstance,
         LPSTR cmdLine, int showCmd)
{
    //MessageBox(0, "Hello Handmade Hero",
    //           "HMH #001", MB_OK | MB_ICONINFORMATION);

    WNDCLASS WindowClass = {};
    
    Win32ResizeDIBSection( &globalBackBuffer, 1280, 720);

    WindowClass.style = CS_HREDRAW | CS_VREDRAW;
    WindowClass.lpfnWndProc = Win32MainWindowCallback;
    WindowClass.hInstance = instance;
    //WindowClass.hIcon;
    WindowClass.lpszClassName="Handmade Hero WindowClass";

    if(RegisterClass( &WindowClass ))
    {
        HWND windowHandle = CreateWindowEx
        (
            0, WindowClass.lpszClassName,
            "Handmade Hero",
            WS_OVERLAPPEDWINDOW | WS_VISIBLE,
            CW_USEDEFAULT, CW_USEDEFAULT,
            CW_USEDEFAULT, CW_USEDEFAULT,
            0, 0,
            instance, 0
        );

        if(windowHandle)
        {
            int offsetX = 0;
            int offsetY = 0;
            isRunning = true;
            while(isRunning)
            {
                MSG message;
                while(PeekMessage( &message, 0, 0, 0, PM_REMOVE ))
                {
                    if(message.message == WM_QUIT)
                    {
                        isRunning = false;
                    }
                    TranslateMessage( &message );
                    DispatchMessage( &message );
                }
                 
                RenderWeirdGradient(globalBackBuffer, offsetX, offsetY );
                HDC deviceContext = GetDC( windowHandle );
                win32WindowDimension dim = Win32GetWindowDimension( windowHandle );
                Win32BlitToWindow( deviceContext, dim.dimX, dim.dimY, globalBackBuffer, 0, 0, dim.dimX, dim.dimY);
                ReleaseDC( windowHandle, deviceContext);

                ++offsetX;
                ++offsetY;
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
