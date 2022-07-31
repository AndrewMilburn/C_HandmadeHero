// win32_handmade.cpp

#include <windows.h>
#include <stdint.h>
#include <xinput.h>


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

#define XInputGetState XInputGetState_
#define XInputSetState XInputSetState_

// Get Controller states ----------------------------------------------------------------------
#define X_INPUT_GET_STATE(name) DWORD WINAPI name( DWORD dwUserIndex, XINPUT_STATE* pState )
typedef X_INPUT_GET_STATE( x_input_get_state );
X_INPUT_GET_STATE( xInputGetStateStub )
{
    return(0);
}
globalVar x_input_get_state* XInputGetState_ = xInputGetStateStub;
// --------------------------------------------------------------------------------------------
// Set Controller states ----------------------------------------------------------------------
#define X_INPUT_SET_STATE(name) DWORD WINAPI name( DWORD dwUserIndex, XINPUT_VIBRATION* pVibration )
typedef X_INPUT_SET_STATE( x_input_set_state );
X_INPUT_SET_STATE( xInputSetStateStub )
{
    return(0);
}
globalVar x_input_set_state* XInputSetState_ = xInputSetStateStub;
// --------------------------------------------------------------------------------------------

struct win32BackBuffer
{
    // NOTE: Pixels are alwasy 32-bits wide, Memory Order BB GG RR XX
    BITMAPINFO bitmapInfo;
    void* bitmapMemory;
    int bitmapWidth;
    int bitmapHeight;
    int bitmapPitch;
};

struct win32WindowDimension
{
    int dimX;
    int dimY;
};

// todo This is a global for now, but in future we'll call it from within the program
globalVar bool GlobalRunning;
globalVar win32BackBuffer globalBackBuffer;

internal void
Win32LoadXInput( void )
{
    HMODULE LoadLibrary(LPCWSTR lpLibFileName);
}

internal win32WindowDimension
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
    int bytesPerPixel = 4;
    // NOTE: When the biHeight field is negative, this is the clue to
    // Windows to treat this bitmap as top-down, not bottom-up, meaning that
    // the first three bytes of the image are the color for the top left pixel
    // in the bitmap, not the bottom left!

    buffer->bitmapInfo.bmiHeader.biSize = sizeof( buffer->bitmapInfo.bmiHeader );
    buffer->bitmapInfo.bmiHeader.biWidth = buffer->bitmapWidth;
    buffer->bitmapInfo.bmiHeader.biHeight = -buffer->bitmapHeight;
    buffer->bitmapInfo.bmiHeader.biPlanes = 1;
    buffer->bitmapInfo.bmiHeader.biBitCount = 32;
    buffer->bitmapInfo.bmiHeader.biCompression = BI_RGB;

    int bitmapMemorySize = bytesPerPixel * (buffer->bitmapWidth * buffer->bitmapHeight);
    buffer->bitmapMemory = VirtualAlloc(0, bitmapMemorySize, MEM_COMMIT, PAGE_READWRITE);
    // todo - Probably Clear this to Black later
    buffer->bitmapPitch = width * bytesPerPixel;
}

// Write From the Back-Buffer
internal void
Win32BlitToWindow(HDC deviceContext, int windowWidth, int windowHeight,
                  win32BackBuffer buffer)
{
    // todo: Aspect Ratio correction
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
        case WM_CLOSE:
        {
            // todo Handle this with message to user / saving code etc.
            GlobalRunning = false;
            break;
        }
        case WM_ACTIVATEAPP:
        {
            break;
        }
        case WM_DESTROY:
        {
            // todo Handle this with message to user / saving code etc.
            GlobalRunning = false;
            break;
        }
        case WM_PAINT:
        {
            PAINTSTRUCT paint;
            HDC deviceContext = BeginPaint(window, &paint);

            win32WindowDimension dim = Win32GetWindowDimension( window );
            Win32BlitToWindow(deviceContext, dim.dimX, dim.dimY, globalBackBuffer);
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
    WNDCLASS WindowClass = {};
    
    Win32ResizeDIBSection( &globalBackBuffer, 1280, 720);

    WindowClass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
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
            // NOTE(casey): Since we specified CS_OWNDC, we can just
            // get one device context and use it forever because we
            // are not sharing it with anyone.
            HDC deviceContext = GetDC( windowHandle );

            int offsetX = 0;
            int offsetY = 0;
            GlobalRunning = true;
            while(GlobalRunning)
            {
                MSG message;
                while(PeekMessage( &message, 0, 0, 0, PM_REMOVE ))
                {
                    if(message.message == WM_QUIT)
                    {
                        GlobalRunning = false;
                    }
                    TranslateMessage( &message );
                    DispatchMessage( &message );
                }

                // todo: Should we poll this more frequently
                for(DWORD controllerIndex = 0; controllerIndex < XUSER_MAX_COUNT; controllerIndex++)
                {
                    XINPUT_STATE controllerState;
                    if(XInputGetState( controllerIndex, &controllerState ) == ERROR_SUCCESS)
                    {
                        // Controller is plugged in
                        // todo: See if DWORD dwPacketNumber; updates too rapidly
                        XINPUT_GAMEPAD* pad = &controllerState.Gamepad;
                        bool padUp = (pad->wButtons & XINPUT_GAMEPAD_DPAD_UP);
                        bool padDown = (pad->wButtons & XINPUT_GAMEPAD_DPAD_DOWN);
                        bool padLeft = (pad->wButtons & XINPUT_GAMEPAD_DPAD_LEFT);
                        bool padRight = (pad->wButtons & XINPUT_GAMEPAD_DPAD_RIGHT);
                        bool padStart = (pad->wButtons & XINPUT_GAMEPAD_START);
                        bool padBack = (pad->wButtons & XINPUT_GAMEPAD_BACK);
                        bool padLeftShoulder = (pad->wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER);
                        bool padRightShoulder = (pad->wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER);
                        bool padA = (pad->wButtons & XINPUT_GAMEPAD_A);
                        bool padB = (pad->wButtons & XINPUT_GAMEPAD_B);
                        bool padX = (pad->wButtons & XINPUT_GAMEPAD_X);
                        bool padY = (pad->wButtons & XINPUT_GAMEPAD_Y);
                        int16 padStickX = pad->sThumbLX;
                        int16 padStickY = pad->sThumbLY;
                    }
                    else
                    {
                        // Controller not available
                    }
                }
                 
                RenderWeirdGradient(globalBackBuffer, offsetX, offsetY );
                HDC deviceContext = GetDC( windowHandle );
                win32WindowDimension dim = Win32GetWindowDimension( windowHandle );
                Win32BlitToWindow( deviceContext, dim.dimX, dim.dimY, globalBackBuffer);
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
