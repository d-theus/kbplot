#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QGLWidget>
#include <QtOpenGL>
#include <map>
#include <string>

#include "event.h"
#include "aglwidget.h"
#include "graphical_object.h"


using std::string;
using std::vector;
using std::map;

class GLWidget : public QGLWidget, public AGLWidget
{
	Q_OBJECT

	int mouseX, mouseY;
	map<string, GraphicalObject*> objects;
	vector<IMouseEventListener*> mouseListeners;

public:
	GLWidget();
	virtual ~GLWidget(){};

	//Qt part
	void initializeGL();
	void resizeGL(int nw, int nh);
	void paintGL();

	//Adapter part
	virtual void GLpaint();
	virtual void GLresize(int nw, int nh);
	virtual void GLupdate();
	virtual void GLinitialize();

	//Qt part
	void mouseMoveEvent(QMouseEvent *e);
	void mouseReleaseEvent(QMouseEvent *e);
	void wheelEvent(QWheelEvent *e);
	//Adapter part
	virtual void subscribeToMouse(IMouseEventListener*);

	virtual double trScreenToGLx(int);
	virtual double trScreenToGLy(int);
	virtual int trGLToScreenx(double);
	virtual int trGLToScreeny(double);

	virtual void addObject(string, GraphicalObject*);
	virtual void clearScene(){};


	virtual void setWorkingArea(double xmin, double xmax, double ymin, double ymax);
};




#endif // GLWIDGET_H
