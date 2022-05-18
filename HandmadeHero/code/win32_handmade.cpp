// win32_handmade.cpp

#include <Windows.h>

LRESULT CALLBACK MainWindowCallback
(
    HWND window,
    UINT message,
    WPARAM wParam,
    LPARAM lParam
)
{
    LRESULT result = 0;
    switch (message)
    {
        case WM_SIZE:
        {
            OutputDebugStringA("WM_SIZE\n");
            break;
        }

        case WM_DESTROY:
        {
            OutputDebugStringA("WM_DESTROY\n");
            break;
        }

        case WM_CLOSE:
        {
            OutputDebugStringA("WM_CLOSE\n");
            break;
        }

        case WM_ACTIVATEAPP:
        {
            OutputDebugStringA("WM_ACTIVATEAPP\n");
            break;
        }   

        case WM_PAINT:
        {
            PAINTSTRUCT paint;
            HDC deviceContext = BeginPaint(window, &paint);
            
            int x = paint.rcPaint.left;
            int y = paint.rcPaint.top;
            int w = paint.rcPaint.right - paint.rcPaint.left;
            int h = paint.rcPaint.bottom - paint.rcPaint.top;
            static DWORD operation = WHITENESS;
            
            PatBlt(deviceContext, x, y, w, h, operation);
            if (operation == WHITENESS)
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

        default :
        {
            //OutputDebugStringA("Default\n");
            result = DefWindowProcA(window, message, wParam, lParam);
            break;
        }
    }
    return(result);
}

int CALLBACK
WinMain
(
    HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    PSTR lpCmdLine,
    INT nCmdShow
)

{
    WNDCLASS WindowClass = {};

        WindowClass.lpfnWndProc = MainWindowCallback;
        WindowClass.hInstance = hInstance;
        WindowClass.lpszClassName = "Handmade Hero Window Class";
        
        if (RegisterClass(& WindowClass))
        {
            HWND windowHandle = CreateWindowEx
            (
                0,
                WindowClass.lpszClassName,
                "Handmade Hero",
                WS_OVERLAPPEDWINDOW|WS_VISIBLE,
                CW_USEDEFAULT,
                CW_USEDEFAULT,
                CW_USEDEFAULT,
                CW_USEDEFAULT,
                0,
                0,
                hInstance,
                0
            );
            if (windowHandle)
            {
                for (;;)
                {
                    MSG message;

                    BOOL messageResult = (GetMessage(&message, 0, 0, 0));
                    if(messageResult > 0)
                    {
                        TranslateMessage(&message);
                        DispatchMessage(&message);
                    }
                    else
                    {
                        break;
                    }
                }
            }
            else
            {
                // TODO: Log a fail?
            }
        }
        else
        {
            // TODO: Log a fail?
        }

    return 0;
}