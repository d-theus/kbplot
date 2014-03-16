//---------------------------------------------------------------------------

#ifndef glwidgetH
#define glwidgetH

//---------------------------------------------------------------------------
#include <map>
#include <string>
#include <vector>

#include "event.h"
#include "aglwidget.h"
#include "graphical_object.h"

using std::string;
using std::vector;
using std::map;

/// структура, содержащая необходимые для
/// создания openGL окна поля
struct GLWindowData
{
    HWND      hWndParent;     // handle to parent window	   	// указатель на окно родителя( MainWindowData)
    LPWSTR 	  title;
    LPWSTR 	  className;      // name of window class
    int x;                    // window position X
    int y;                    // window position Y
    int width;
    int height;
    int bits; //число бит на ,,,,
};

struct InternalGLWindowData
{
    LPWSTR 	  className;      // name of window class
    HGLRC     hRC;		      // Permanent Rendering Context	// порт для связи OpenGL и системы
    HDC	      hDC;		      // Private GDI Device Context
    HWND      hWnd;		      // Holds Our Window Handle
    HINSTANCE hInstance;	  // Holds The Instance Of The GL window
	GLWindowData data;
};


class GLWidget : public AGLWidget
{
public:
	GLWidget();
	virtual ~GLWidget();

    ///////////////////////////////////////////////////////////////
    // GL Window
	HWND CreateGLWindow(const GLWindowData wndData, WNDPROC WndProc, LPVOID WndProcData);
	void KillGLWindow();
	bool TryProcessMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& res);

private:
	// gl funcs	 /* ex Qt part */
	bool initializeGL();
	void resizeGL(int nw, int nh);
	void paintGL();
public:
	//Adapter part
    virtual void GLpaint();
	virtual void GLresize(int nw, int nh);
	virtual void GLupdate();
	virtual bool GLinit();
	virtual void mousePressedEvent(int xPos, int yPos);
	virtual void mouseMoveEvent(int xPos, int yPos);
	virtual void mouseReleaseEvent(int xPos, int yPos);
	virtual void wheelEvent(int delta);
	//Adapter part
	virtual void subscribeToMouse(IMouseEventListener*);

	virtual double trScreenToGLx(int);
	virtual double trScreenToGLy(int);
	virtual int trGLToScreenx(double);
	virtual int trGLToScreeny(double);


	virtual void addLayer(const string &key);
	virtual void addObject(const string &key, GraphicalObject* obj);

	virtual void removeLayer(const string &key);

	virtual void clearLayer(const string &key);
	virtual void clearScene();

    // очистка контейнеров
    // используется в примере,
    // вызывается перед сменой примера
    // и перед удалением окна
	virtual void clearContainers();

	virtual void setWorkingArea(double xmin, double xmax, double ymin, double ymax);

	virtual void exportAsImage(const wchar_t* filename);

public:
	int width();
	int height();
private:
	int mouseX, mouseY;
	map<string, vector<GraphicalObject*> >objects;
	vector<IMouseEventListener*> mouseListeners;
	int w;
    int h;

	static bool active;				// Window Active Flag Set To TRUE By Default
	InternalGLWindowData itlWndData;
};

#endif // GLWIDGET_H

