///////////////////////////////////////////////////////////////////////////////
/*
 *  Класс окна для MS Windows.
 */
#pragma hdrstop
#include "h/Window.h"
///////////////////////////////////////////////////////////////////////////////
#pragma package(smart_init)

#define OFFSET 5;
#define OFFSETR 100;

Window::Window(GLWidget *view, ModelPlot *model, int width, int height, HINSTANCE hInstance, int nCmdShow) :
			   cmdShow(nCmdShow)
{
     data.view = view;
     data.model = model;

	 wData.hWnd   = NULL;
	 wData.title  = L"KbPlot";
	 wData.className = L"KbPlot_Win_Class";
     wData.x 	  = 0;
     wData.y      = 0;
     wData.width  = width;
	 wData.height = height;
	 wData.hInstance = hInstance;
     glData.title  = L"KbPlot";
     glData.className = L"OpenGL_Win_KbPlot";
     glData.bits  = 16;
     glData.x	  = 0;
     glData.y     = 0;
     glData.width = width   - OFFSETR;
     glData.height = height - OFFSETR;
}

Window::~Window()
{

}
///////////////////////////////////////////////////////////////////////////////
int Window::Create()
{
	bool success = data.view->GLinit();

    if (!success)
    {
        MessageBox(NULL,L"GL Initialization Failed.",L"ERROR",MB_OK|MB_ICONEXCLAMATION);
        return FALSE;
    }

    bool isWinMain = CreateMainWindow();
	glData.hWndParent = wData.hWnd;
    HWND glHWND = data.view->CreateGLWindow(glData, WndProc, (LPVOID) &data);
    if (!isWinMain || !glHWND)
    {
    	return FALSE;
    }

    // Задаем размер окна в соответствии с его содержимым
    RECT rect;
    ::GetWindowRect(glHWND, &rect);      // get size of glWin
    rect.right += 80 + OFFSET;	// + ширина кнопок
	::AdjustWindowRectEx(&rect, wData.winStyle, FALSE, wData.winStyleEx);//выравнивание
	::SetWindowPos(wData.hWnd, 0, 100, 100, rect.right-rect.left, rect.bottom-rect.top, SWP_NOZORDER);
    //-------------------------

    ::ShowWindow(wData.hWnd,  cmdShow);
    ::ShowWindow(glHWND, cmdShow);					// Show The Window
	//::SetForegroundWindow(glHWND);				    // Slightly Higher Priority
    ::SetFocus(glHWND);							// Sets Keyboard Focus To The Window

    return TRUE;
}
int Window::Exec()
{
	MSG msg;				   							// Windows Message Structure
	BOOL  done=FALSE;                        			// Bool Variable To Exit Loop
	while(!done)                                		// Loop That Runs Until done=TRUE
	{
		if (PeekMessage(&msg,NULL,0,0,PM_REMOVE))   	// Is There A Message Waiting?
		{
			switch(msg.message)
			{
				case WM_QUIT:
					done=TRUE;                  			// If So done=TRUE
				break;
				default:
					LRESULT res;
					data.view->TryProcessMessage(msg.message, msg.wParam, msg.lParam, res);
			}
				TranslateMessage(&msg);             	// Translate The Message
				DispatchMessage(&msg);					// Dispatch The Message
		}
	}
	data.view->KillGLWindow();									// Kill The GL Window
    KillMainWindow();                               // Kill The Main Window
    return (msg.wParam);                            // Exit The Program
}
///////////////////////////////////////////////////////////////////////////////
// Main Window
///////////////////////////////////////////////////////////////////////////////
GLvoid Window::KillMainWindow()
{
	::UnregisterClass(wData.className, wData.hInstance);
    wData.hInstance = NULL;
}
///////////////////////////////////////////////////////////////////////////////
BOOL Window::CreateMainWindow()
{
     wData.hWnd 	  = 0;
     wData.winStyle   = WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN;
     wData.winStyleEx = WS_EX_WINDOWEDGE;
     wData.x 		  = CW_USEDEFAULT;
     wData.y 		  = CW_USEDEFAULT;
     wData.width 	  = CW_USEDEFAULT;
	 wData.height 	  = CW_USEDEFAULT;

	WNDCLASSEX winClass; 		// window class information
	// populate window class struct
	winClass.cbSize        = sizeof(WNDCLASSEX);
	winClass.style         = 0;                           	    // class styles: CS_OWNDC, CS_PARENTDC, CS_CLASSDC, CS_GLOBALCLASS, ...
	winClass.lpfnWndProc   = (WNDPROC)WndProc;    		    // pointer to window procedure
    winClass.cbClsExtra    = 0;
    winClass.cbWndExtra    = 0;
    winClass.hInstance     = wData.hInstance;                              	// owner of this class
    winClass.hIcon         = LoadIcon(wData.hInstance, IDI_APPLICATION);    // default icon
    winClass.hIconSm       = 0;
    winClass.hCursor       = LoadCursor(0, IDC_ARROW);              		// default arrow cursor
    winClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);   		// default white brush
    winClass.lpszMenuName  = 0;
    winClass.lpszClassName = wData.className;
    winClass.hIconSm       = LoadIcon(wData.hInstance, IDI_APPLICATION);    // default small icon

    // register a window class
    if(!::RegisterClassEx(&winClass))
    {
        MessageBox(NULL,L"Failed To Register The Window Class.",L"ERROR",MB_OK|MB_ICONEXCLAMATION);
        return FALSE;
    }


    wData.hWnd = ::CreateWindowEx(
    							wData.winStyleEx,           // window border with a sunken edge
                              	wData.className,            // name of a registered window class
                              	wData.title,                // caption of window
                              	wData.winStyle,             // window style
                              	wData.x,                    // x position
                              	wData.y,                    // y position
                              	wData.width,                // witdh
                              	wData.height,               // height
                              	0,         	    			// handle to parent window
                              	0,           				// handle to menu
                              	wData.hInstance,            // application instance
                                (LPVOID)&data);    		    // window creation data

    if(!wData.hWnd)
    {
    	KillMainWindow();
        MessageBox(NULL,L"Window Creation Error.",L"ERROR",MB_OK|MB_ICONEXCLAMATION);
        return FALSE;
    }

     // Создание 3-х радиокнопок.
     const int posX = glData.width + OFFSET;
	 const int posY = (int)OFFSET;
   CreateWindow(L"button", L"View 1",
      WS_CHILD|WS_VISIBLE|BS_AUTORADIOBUTTON/*BS_RADIOBUTTON*/ ,
      posX, posY, 80, 30, wData.hWnd, (HMENU)10001, wData.hInstance, NULL);
   CreateWindow(L"button", L"View 2",
      WS_CHILD|WS_VISIBLE|BS_AUTORADIOBUTTON ,
      posX, posY + 30, 80, 30, wData.hWnd, (HMENU)10002, wData.hInstance, NULL);
   CreateWindow(L"button", L"View 3",
      WS_CHILD|WS_VISIBLE|BS_AUTORADIOBUTTON ,
      posX, posY + 60, 80, 30, wData.hWnd, (HMENU)10003, wData.hInstance, NULL);
   CreateWindow(L"button", L"View 4",
      WS_CHILD|WS_VISIBLE|BS_AUTORADIOBUTTON,
      posX, posY + 90, 80, 30, wData.hWnd, (HMENU)10004, wData.hInstance, NULL);
   CreateWindow(L"button", L"View 5",
      WS_CHILD|WS_VISIBLE|BS_AUTORADIOBUTTON,
      posX, posY + 120, 80, 30, wData.hWnd, (HMENU)10005, wData.hInstance, NULL);

     	//  Создание кнопки сохранения рисунка

    CreateWindow(
           L"BUTTON", /* this makes a "button" */
           L"Save Graph", /* this is the text which will appear in the button */
           WS_VISIBLE | WS_CHILD,
           posX, /* these four lines are the position and dimensions of the button */
           posY + 150,
           80,
           30,
           wData.hWnd, /* this is the buttons parent window */
           (HMENU)10006, /* these next two lines pretty much tell windows what to do when the button is pressed */
           wData.hInstance,
           NULL);

   return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
LRESULT CALLBACK Window::WndProc(   HWND    hWnd,// Handle For This Window
				UINT    uMsg,                    // Message For This Window
				WPARAM  wParam,                  // Additional Message Information
				LPARAM  lParam)                  // Additional Message Information
{

	// Так как ф-ция статична, нужно получить необходимые данные (указатели на GLWidget и ModelPlot) из параметров ф-ции
	////////////////////
	static MVData *data;
	data = (MVData*)::GetWindowLongPtr(hWnd, GWL_USERDATA);

    // Получаем структуру MVData* c указателями на GLWidget и ModelPlot
    if(uMsg == WM_NCCREATE)  // Non-Client Create
    {
        // WM_NCCREATE message is called before non-client parts(border,
		// titlebar, menu,etc) are created. This message comes with a pointer
        // to CREATESTRUCT in lParam. The lpCreateParams member of CREATESTRUCT
        // actually contains the value of lpPraram of CreateWindowEX().
        // First, retrieve the pointrer to the controller specified when
        // Win::Window is setup.
		data = (MVData*)(((CREATESTRUCT*)lParam)->lpCreateParams);
		if(data)
		{
			// Store the pointer to the GLWidget into GWL_USERDATA,
			// so, other messege can be routed to the associated Controller.
			::SetWindowLongPtr(hWnd, GWL_USERDATA, (LONG_PTR)data);
		}
		else
			MessageBox(NULL,L"[ERROR] Failed to create controller::windowProcedure().",L"",MB_OK|MB_ICONQUESTION);

        return ::DefWindowProc(hWnd, uMsg, wParam, lParam);
    }
    ////////////////////
    // check NULL pointer, because GWL_USERDATA is initially 0, and
    // we store a valid pointer value when WM_NCCREATE is called.
    if(!data)
        return ::DefWindowProc(hWnd, uMsg, wParam, lParam);

	switch (uMsg)                               // Check For Windows Messages
	{
    	case WM_COMMAND:
        {
            // radiobuttons
            if(LOWORD(wParam) >= 10001 && LOWORD(wParam) <= 10005)
            {
                // очистка контейнеров в glWidget
                data->view->clearContainers();
                // Если мы нажали на 1-й радиокнопке.
                if(LOWORD(wParam)==10001)
                    data->model->CreateOption1();
                // Если мы нажали на 2-й радиокнопке.
                if(LOWORD(wParam)==10002)
                    data->model->CreateOption2();
                // Если мы нажали на 3-й радиокнопке.
                if(LOWORD(wParam)==10003)
                    data->model->CreateOption3();
                if(LOWORD(wParam)==10004)
                    data->model->CreateOption4();
                if(LOWORD(wParam)==10005)
                    data->model->CreateOption5();
                data->view->GLupdate();
            }
            else
            if(LOWORD(wParam)==10006) //нажатие на копку
            {
                OPENFILENAME ofn;
                wchar_t szFileName[MAX_PATH] = L" ";
                ZeroMemory (&ofn, sizeof(ofn));
                ofn.lStructSize = sizeof(ofn);
                ofn.hwndOwner = hWnd;
                ofn.lpstrFilter = L"Png Files (*.png)\0*.png\0";
                ofn.lpstrFile = szFileName;
                ofn.nMaxFile = MAX_PATH;
                ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST;
                ofn.lpstrDefExt = L"png";
                if(GetSaveFileName(&ofn))
                {
                    data->view->exportAsImage(szFileName);
                }
             }
        	return 0;
    	}

		default:
		{
			LRESULT res;
			if(data->view->TryProcessMessage(uMsg, wParam, lParam, res))
				return res;
		}
	}
	 // Pass All Unhandled Messages To DefWindowProc
    return DefWindowProc(hWnd,uMsg,wParam,lParam);
}
///////////////////////////////////////////////////////////////////////////////
// load an icon using resource ID and convert it to HICON
///////////////////////////////////////////////////////////////////////////////
HICON Window::loadIcon(HINSTANCE hInstance,int id)
{
    return (HICON)::LoadImage(hInstance, MAKEINTRESOURCE(id), IMAGE_ICON, 0, 0, LR_DEFAULTSIZE);
}
///////////////////////////////////////////////////////////////////////////////
// load an icon using resource ID and convert it to HICON
///////////////////////////////////////////////////////////////////////////////
HICON Window::loadCursor(HINSTANCE hInstance,int id)
{
    return (HCURSOR)::LoadImage(hInstance, MAKEINTRESOURCE(id), IMAGE_CURSOR, 0, 0, LR_DEFAULTSIZE);
}
