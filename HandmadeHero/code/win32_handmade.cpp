// win32_handmade.cpp

#include <windows.h>

LRESULT CALLBACK mainWindowCallback(HWND window, UINT message,
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
        case WM_PAINT:
        {
            PAINTSTRUCT paint;
            HDC deviceContext = BeginPaint(window, &paint);
            int paintX = paint.rcPaint.left;
            int paintY = paint.rcPaint.top;
            int paintWidth = paint.rcPaint.right - paintX;
            int paintHeight = paint.rcPaint.bottom - paintY;
            static DWORD operation = WHITENESS;
            PatBlt( deviceContext, paintX, paintY, paintWidth, paintHeight, operation);
            if(operation == WHITENESS)
            {
                operation = BLACKNESS;
            }
            else
            {
                operation = WHITENESS;
            }
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
    //todo: Check whether the below flags are needed
    WindowClass.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
    WindowClass.lpfnWndProc = mainWindowCallback;
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
            for(;;)
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
