#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QGLWidget>
#include <QtOpenGL>
#include <map>
#include <string>

#include "event.h"
#include "graphical_object.h"


using std::string;
using std::vector;
using std::map;

class GLWidget : public QGLWidget
{
	Q_OBJECT

	int mouseX, mouseY;
	map<string, GraphicalObject*> objects;
	vector<IMouseEventListener*> mouseListeners;

public:
	GLWidget(QWidget *parent = 0);
	void initializeGL();
	void resizeGL(int nw, int nh);
	void paintGL();

	void mouseMoveEvent(QMouseEvent *e);
	void mouseReleaseEvent(QMouseEvent *e);

	double scr_to_gl_x(int);
	double scr_to_gl_y(int);
	int gl_to_scr_x(double);
	int gl_to_scr_y(double);

	void addObject(string, GraphicalObject*);
	void deleteObject(int);

	void subscribeToMouse(IMouseEventListener*);

	void setWorkingArea(double xmin, double xmax, double ymin, double ymax);
};




#endif // GLWIDGET_H
