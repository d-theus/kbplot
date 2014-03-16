//---------------------------------------------------------------------------
#pragma hdrstop

#include "h/Window.h"
#include "h/ModelPlot.h"

#include <tchar.h>
#pragma argsused

WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
    GLWidget view;
    ModelPlot model(&view);
    Window app(&view, &model, 1024, 768, hInstance, nCmdShow);
    app.Create();
    return app.Exec();	//вход в цикл отслеживания сообщений
}



