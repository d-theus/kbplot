//---------------------------------------------------------------------------


#pragma hdrstop

#include "h/glwidget.h"
#include <iostream>
//---------------------------------------------------------------------------
#define DEBUG              // delete on release
#pragma package(smart_init)

void SaveScreenShot(const wchar_t* filename, const size_t width, const size_t height);

GLWidget::GLWidget() : w(0), h(0)
{
    initializeGL();
}

GLWidget::~GLWidget()
{
    clearContainers();
}

void GLWidget::clearScene()
{
    for(map<string, vector<GraphicalObject*> >::iterator layer =
                objects.begin(); layer != objects.end(); layer++)
    {

        for(vector<GraphicalObject*>::iterator o =
                    layer->second.begin(); o != layer->second.end(); o++)
        {
            delete *o;
        }
        layer->second.clear();
    }
#ifdef DEBUG
    std::cout << "glwidget::clearing collection"<<std::endl;
#endif
    objects.clear();
}

void GLWidget::clearContainers()
{
    clearScene();
    mouseListeners.clear();
}

void GLWidget::GLpaint()
{
    this -> paintGL();
}
bool GLWidget::GLinit()
{
    return this -> initializeGL();
}
void GLWidget::GLresize(int nw, int nh)
{
    this -> resizeGL(this->w = nw, this->h = nh);
}
void GLWidget::GLupdate()
{
    // this -> updateGL();
    this -> GLpaint();
}

bool GLWidget::initializeGL()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    return true;
}

void GLWidget::paintGL()
{

#ifdef DEBUG
    std::cout << "paintGL()"<<std::endl;
#endif

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    for(map<string, vector<GraphicalObject*> >::iterator layer =
                objects.begin(); layer != objects.end(); layer++)
    {

        for(vector<GraphicalObject*>::iterator o =
                    layer->second.begin(); o != layer->second.end(); o++)
        {
            (*o)->draw();
        }
    }

    glFlush();
}

void GLWidget::resizeGL(int nw, int nh)
{
#ifdef DEBUG
    std::cout << "resizeGL()"<<std::endl;
#endif
    glViewport(0,0, this->w = nw, this->h = nh);
}

void GLWidget::mouseMoveEvent(int xPos, int yPos)
{

    for(std::vector<IMouseEventListener*>::const_iterator i = this->mouseListeners.begin(); i != mouseListeners.end(); ++i)
    {
        (*i)->mouseMoveEvent(xPos, yPos);
    }
}

void GLWidget::mouseReleaseEvent(int xPos, int yPos)
{

    for(std::vector<IMouseEventListener*>::const_iterator i = this->mouseListeners.begin(); i != mouseListeners.end(); ++i)
    {
        (*i)->mouseReleaseEvent(xPos, yPos);
    }
}

void GLWidget::mousePressedEvent(int xPos, int yPos)
{
    for(std::vector<IMouseEventListener*>::const_iterator i = this->mouseListeners.begin(); i != mouseListeners.end(); ++i)
    {
        (*i)->mousePressedEvent(xPos, yPos);
    }
}


void GLWidget::wheelEvent(int delta)
{

    for(std::vector<IMouseEventListener*>::const_iterator i = this->mouseListeners.begin(); i != mouseListeners.end(); i++)
    {
        (*i)->mouseScrollEvent(delta);
    }
}

void GLWidget::addLayer(const string &key)
{
    if(objects.find(key) == objects.end())
    {
        objects[key] = vector<GraphicalObject*>();
    }
    else
    {
        fprintf(stderr, "Layer already exists\n");
    }
}

void GLWidget::addObject(const string &key, GraphicalObject *obj)
{
    map<string, vector<GraphicalObject*> >::iterator layer = objects.find(key);
    if(objects.end() != layer)
    {
        layer->second.push_back(obj);
    }
    else
    {
        fprintf(stderr, "No such layer\n");
    }
}

double GLWidget::trScreenToGLx(int x)
{
    return (double)(this->w - x)/this->w;
}

double GLWidget::trScreenToGLy(int y)
{
    return (double)(this->h - y)/this->h;
}

int GLWidget::trGLToScreenx(double x)
{
    return (double)(this->w) * (x - 1.0);
}

int GLWidget::trGLToScreeny(double y)
{
    return (double)(this->w) * (y - 1.0);
}

void GLWidget::subscribeToMouse(IMouseEventListener*l)
{
    this->mouseListeners.push_back(l);
}

void GLWidget::setWorkingArea(double xmin, double xmax, double ymin, double ymax)
{
#ifdef DEBUG
    std::cout << "setWorkingArea("<<xmin<<","<<xmax<<","<<ymin<<","<<ymax<<")"<<std::endl;
#endif
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(xmin, xmax, ymin, ymax, -1.0, 0.0);
}

void GLWidget::exportAsImage(const wchar_t* filename)
{
    SaveScreenShot(filename, 1024/*this->w*/, this->h);
}

void GLWidget::removeLayer(const string &key)
{
    clearLayer(key);
    objects.erase(key);
}


void GLWidget::clearLayer(const string &key)
{
    map<string, vector<GraphicalObject*> >::iterator layer = objects.find(key);
    if(objects.end() != layer)
    {
        for(std::vector<GraphicalObject*>::iterator i =
                    layer->second.begin(); i != layer->second.end(); i++)
        {
            delete *i;
        }
        layer->second.clear();
    }
    else
    {
        fprintf(stderr, "No such layer\n");
    }
}

int GLWidget::width()
{
    return this->w;
}
int GLWidget::height()
{
    return this->h;
}



///////////////////////////////////////////////////////////////////////////////
// GL Window
///////////////////////////////////////////////////////////////////////////////
HWND GLWidget::CreateGLWindow(const GLWindowData wndData, WNDPROC WndProc, LPVOID WndProcData)
{
    GLuint      PixelFormat;					// Holds The Results After Searching For A Match
    WNDCLASS  wc;	     						// Windows Class Structure


    RECT WindowRect;                            // Grabs Rectangle Upper Left / Lower Right Values
    WindowRect.left   =(long)wndData.x;          // Set Left Value To 0
    WindowRect.right  =(long)wndData.width;      // Set Right Value To Requested Width
    WindowRect.top	  =(long)wndData.y;          // Set Top Value To 0
    WindowRect.bottom =(long)wndData.height;     // Set Bottom Value To Requested Height

    itlWndData.className = wndData.className;
    itlWndData.hInstance = GetModuleHandle(NULL);  			// Grab An Instance For Our Window
    wc.style  = CS_OWNDC;
    wc.lpfnWndProc = (WNDPROC)WndProc;					// WndProc Handles Messages
    wc.cbClsExtra = 0;									// No Extra Window Data
    wc.cbWndExtra = 0;									// No Extra Window Data
    wc.hInstance = itlWndData.hInstance;				   	// Set The Instance
    wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);				// Load The Default Icon
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);			// Load The Arrow Pointer
    wc.hbrBackground = NULL;							// No Background Required For GL
    wc.lpszMenuName = NULL;								// We Don't Want A Menu
    wc.lpszClassName = wndData.className;   					// Set The Class Name

    if (!RegisterClass(&wc))							// Attempt To Register The Window Class
    {
        MessageBox(NULL,L"Failed To Register The Window Class.",L"ERROR",MB_OK|MB_ICONEXCLAMATION);
        return FALSE;									// Exit And Return FALSE
    }

    const DWORD dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;   // Window Extended Style
    const DWORD dwStyle   = WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN                                                   ;                  // Windows Style

    if (!(itlWndData.hWnd=CreateWindowEx(
                              dwExStyle,        			// Extended Style For The Window
                              wndData.className,		 			// Class Name
                              wndData.title,  			 			// Window Title
                              WS_CLIPSIBLINGS |					// Required Window Style
                              WS_CLIPCHILDREN |				 	// Required Window Style
                              dwStyle,				   		// Selected Window Style
                              wndData.x, wndData.y,		  			// Window Position
                              WindowRect.right-WindowRect.left,	// Calculate Adjusted Window Width
                              WindowRect.bottom-WindowRect.top,	// Calculate Adjusted Window Height
                              wndData.hWndParent,	   				// Parent Window
                              NULL,								// No Menu
                              itlWndData.hInstance,					// Instance
                              WndProcData)))

    {
        KillGLWindow();                         // Reset The Display
        MessageBox(NULL,L"Window Creation Error.",L"ERROR",MB_OK|MB_ICONEXCLAMATION);
        return FALSE;                           // Return FALSE
    }

    static  PIXELFORMATDESCRIPTOR pfd=                  // pfd Tells Windows How We Want Things To Be
    {
        sizeof(PIXELFORMATDESCRIPTOR),                  // Size Of This Pixel Format Descriptor
        1,												// Version Number
        PFD_DRAW_TO_WINDOW |							// Format Must Support Window
        PFD_SUPPORT_OPENGL |							// Format Must Support OpenGL
        PFD_DOUBLEBUFFER,								// Must Support Double Buffering
        PFD_TYPE_RGBA,									// Request An RGBA Format
        wndData.bits,											// Select Our Color Depth
        0, 0, 0, 0, 0, 0,								// Color Bits Ignored
        0,												// No Alpha Buffer
        0,												// Shift Bit Ignored
        0,												// No Accumulation Buffer
        0, 0, 0, 0,										// Accumulation Bits Ignored
        16,												// 16Bit Z-Buffer (Depth Buffer)
        0,												// No Stencil Buffer
        0,												// No Auxiliary Buffer
        PFD_MAIN_PLANE,									// Main Drawing Layer
        0,												// Reserved
        0, 0, 0											// Layer Masks Ignored
    };

    if (!(itlWndData.hDC=GetDC(itlWndData.hWnd)))  	 			// Did We Get A Device Context?
    {
        KillGLWindow();									// Reset The Display
        MessageBox(NULL,L"Can't Create A GL Device Context.",L"ERROR",MB_OK|MB_ICONEXCLAMATION);
        return FALSE;									// Return FALSE
    }
    if (!(PixelFormat=ChoosePixelFormat(itlWndData.hDC,&pfd)))	// Did Windows Find A Matching Pixel Format?
    {
        KillGLWindow();									// Reset The Display
        MessageBox(NULL,L"Can't Find A Suitable PixelFormat.L",L"ERROR",MB_OK|MB_ICONEXCLAMATION);
        return FALSE;									// Return FALSE
    }

    if(!SetPixelFormat(itlWndData.hDC,PixelFormat,&pfd))    // Are We Able To Set The Pixel Format?
    {
        KillGLWindow();									// Reset The Display
        MessageBox(NULL,L"Can't Set The PixelFormat.",L"ERROR",MB_OK|MB_ICONEXCLAMATION);
        return FALSE;									// Return FALSE
    }

    if (!(itlWndData.hRC=wglCreateContext(itlWndData.hDC)))     // Are We Able To Get A Rendering Context?
    {
        KillGLWindow();									// Reset The Display
        MessageBox(NULL,L"Can't Create A GL Rendering Context.",L"ERROR",MB_OK|MB_ICONEXCLAMATION);
        return FALSE;									// Return FALSE
    }
    if(!wglMakeCurrent(itlWndData.hDC, itlWndData.hRC))         // Try To Activate The Rendering Context
    {
        KillGLWindow();									// Reset The Display
        MessageBox(NULL,L"Can't Activate The GL Rendering Context.",L"ERROR",MB_OK|MB_ICONEXCLAMATION);
        return FALSE;									// Return FALSE
    }
    SetTimer(itlWndData.hWnd, 10500, 25, (TIMERPROC) NULL);

    return itlWndData.hWnd;										// Success
}
///////////////////////////////////////////////////////////////////////////////
// Properly Kill The Window
void GLWidget::KillGLWindow()
{
    if (!UnregisterClass(itlWndData.className, itlWndData.hInstance))   	// Are We Able To Unregister Class
    {
        MessageBox(NULL,L"Could Not Unregister Class.",L"SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
        itlWndData.hInstance=NULL;                         				// Set hInstance To NULL
    }
}

bool GLWidget::TryProcessMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& res)
{
    res = 0;

    switch(uMsg)
    {
    case WM_TIMER:
        if(wParam == 10500 && active)
        {
            GLpaint();          // Draw The Scene
            SwapBuffers(itlWndData.hDC);       // Swap Buffers (Double Buffering)
            return true;
        }
        return false;

    case WM_ACTIVATE:                       // Watch For Window Activate Message
    {
        // LoWord Can Be WA_INACTIVE, WA_ACTIVE, WA_CLICKACTIVE,
        // The High-Order Word Specifies The Minimized State Of The Window Being Activated Or Deactivated.
        // A NonZero Value Indicates The Window Is Minimized.
        if ((LOWORD(wParam) != WA_INACTIVE) && !((BOOL)HIWORD(wParam)))
            active=TRUE;						// Program Is Active
        else
            active=FALSE;						// Program Is No Longer Active

        return true;								// Return To The Message Loop
    }

    case WM_SYSCOMMAND:                     // Intercept System Commands
    {
        switch (wParam)                     // Check System Calls
        {
        case SC_SCREENSAVE:             // Screensaver Trying To Start?
        case SC_MONITORPOWER:           // Monitor Trying To Enter Powersave?
            return true;						// Prevent From Happening
        }
        return false;								// Exit
    }

    case WM_CLOSE:                          // Did We Receive A Close Message?
    {
        // Завершим все gl операции
        glFinish();
        // Очистим ресурсы
        clearContainers();
        //TODO data->model->Clear();

        if (itlWndData.hRC)								// Do We Have A Rendering Context?
        {
            if (!wglMakeCurrent(NULL,NULL))     	// Are We Able To Release The DC And RC Contexts?
                MessageBox(NULL,L"Release Of DC And RC Failed.",L"SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);

            if (!wglDeleteContext(itlWndData.hRC))      // Are We Able To Delete The RC?
                MessageBox(NULL,L"Release Rendering Context Failed.",L"SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);

            itlWndData.hRC = NULL;                      // Set RC To NULL
        }

        if (itlWndData.hDC && !ReleaseDC(itlWndData.hWnd, itlWndData.hDC))			// Are We Able To Release The DC
        {
            MessageBox(NULL,L"Release Device Context Failed.",L"SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
            itlWndData.hDC=NULL;											// Set DC To NULL
        }
        if (itlWndData.hWnd && !DestroyWindow(itlWndData.hWnd))       			// Are We Able To Destroy The Window?
        {
            MessageBox(NULL,L"Could Not Release hWnd.",L"SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
            itlWndData.hWnd=NULL;                          					// Set hWnd To NULL
        }
        return true;							// Jump Back
    }

    case WM_DESTROY:
    {
        PostQuitMessage(0);                 // Send A Quit Message
        return true;							// Jump Back
    }

    case WM_SIZE:                           // Resize The OpenGL Window
    {
        // LoWord=Width, HiWord=Height
        int width = LOWORD(lParam);
        int height = HIWORD(lParam);
        if (height==0)				  // Prevent A Divide By Zero By
            height=1;
        GLresize(width, height);
        return true;							// Jump Back
    }

    case WM_LBUTTONDOWN:
    {
        short xPos = LOWORD(lParam);
        short yPos = HIWORD(lParam);
        mousePressedEvent(xPos, yPos);
        return true;
    }

    case WM_LBUTTONUP:
    {
        short xPos = LOWORD(lParam);
        short yPos = HIWORD(lParam);
        mouseReleaseEvent(xPos, yPos);
        return true;
    }
    case WM_MOUSEMOVE:
    {
        short xPos = LOWORD(lParam);
        short yPos = HIWORD(lParam);
        mouseMoveEvent(xPos, yPos);
        return true;
    }
    case WM_MOUSEWHEEL:
    {
        int zDelta = GET_WHEEL_DELTA_WPARAM(wParam);
        wheelEvent(zDelta);
        return true;
    }

    default:
        return false;
    }
}

bool GLWidget::active = true;		// Window Active Flag Set To TRUE By Default
