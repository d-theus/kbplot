/*
	http://msdn.microsoft.com/en-us/library/windows/desktop/aa383751(v=vs.85).aspx
	WinDataTypes:

		HINSTANCE - A handle to an instance. This is the base address of the module in memory.
		HACCEL - A handle to an accelerator table.
		LPSTR - A pointer to a null-terminated string of 8-bit Windows (ANSI) characters.

		http://msdn.microsoft.com/en-us/library/windows/desktop/ms645526(v=vs.85).aspx
		A keyboard accelerator (or, simply, accelerator) is a keystroke or combination of keystrokes that generates a WM_COMMAND or WM_SYSCOMMAND message for an application.


*/

 /*
  * Справочная информация о типах Win API
  * http://msdn.microsoft.com/en-us/library/windows/desktop/aa383751(v=vs.85).aspx
  *
  * HWND - A handle to a window.
  * LPARAM - A message parameter.
  * HDC - A handle to a device context (DC).
  *	    http://msdn.microsoft.com/en-us/library/windows/desktop/dd183560(v=vs.85).aspx
  *		There are four types of DCs: display, printer, memory (or compatible), and information. Each type serves a specific purpose, as described in the following table.
  *		Display - Supports drawing operations on a video display.
  *	WPARAM - A message parameter.
  * HGLRC - http://msdn.microsoft.com/en-us/library/windows/desktop/dd369038(v=vs.85).aspx
  */
///////////////////////////////////////////////////////////////////////////////
/*
 *  Класс окна для MS Windows.
 */
#ifndef WINDOW_H
#define WINDOW_H
//---------------------------------------------------------------------------
#include <windows.h>
#include "GLWidget.h"
#include "ModelPlot.h"

/// структура, содержащая необходимые для
/// создания окна поля
struct MainWindowData
{
	HWND hWnd;                             	// handle to window
    DWORD winStyle;                         // window style: WS_OVERLAPPEDWINDOW, WS_CHILD, ...
    DWORD winStyleEx;                       // extended window style
    LPWSTR title;              				// title of window
    LPWSTR className;          				// name of window class
    int x;                                  // window position X
    int y;                                  // window position Y
    int width;                              // window width
    int height;                             // window height
    HINSTANCE hInstance;                    // handle to instance
};

// Данные для передачи в статическую ф-цию WndProc
struct MVData
{
	GLWidget  *view;
    ModelPlot *model;
    HGLRC     glHRC;		      // Permanent Rendering Context	// порт для связи OpenGL и системы
    HDC	      glHDC;		      // Private GDI Device Context
    HWND      glHWnd;		      // Holds Our Window Handle
};

class Window
{
	public:
		Window(GLWidget *view, ModelPlot *model, int width, int height, HINSTANCE hInstance, int nCmdShow);
		~Window(void);

                // Создание представления
                int Create();
                // Вход в цикл обработки сообщений системы
                int Exec();

        private:
                ///////////////////////////////////////////////////////////////
				// Main Window
				BOOL CreateMainWindow();
                GLvoid KillMainWindow();

                ///////////////////////////////////////////////////////////////
                static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
                ///////////////////////////////////////////////////////////////
        		HICON loadIcon(HINSTANCE hInstance,int id);         // load icon using resource id
        		HCURSOR loadCursor(HINSTANCE hInstance,int id);     // load icon using resource id
        private:
                MainWindowData wData;   		// данные главного окна
                GLWindowData glData;            // данные open gl окна
                MVData data;                    // указатели на GLWidget и ModelPlot
                int cmdShow;                    // вспомогательный параметр из _tMain для функции winapi ::ShowWindow(...);
};

#endif //WINDOW_H
