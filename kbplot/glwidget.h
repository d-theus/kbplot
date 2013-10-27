#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QGLWidget>
#include <QtOpenGL>
#include <map>
#include <string>
#include "event.h"

class Primitive;
class Polyline;

using std::string;

class GLWidget : public QGLWidget
{
	Q_OBJECT

	int mouseX, mouseY;
	std::map<string, Primitive*> objects;
	std::vector<IMouseEventListener*> mouseListeners;

public:
	double xpos, ypos;

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

	void addObject(string, Primitive*);
	void deleteObject(int);

	void subscribeToMouse(IMouseEventListener*);

	void setWorkingArea(double xmin, double xmax, double ymin, double ymax);
};

class Primitive {
	protected:
		bool isTranslated = false;
		double trX, trY;
		void before_draw()const;
		void after_draw()const;
	public:
		virtual ~Primitive(){};
		virtual void draw() const = 0;
		void setTranslation(double,double);
		void setTranslation(bool);
};

class Polyline : public Primitive {
public:
	Polyline(std::vector<double> *);
	virtual void draw() const;
	virtual ~Polyline(){};
private:
	std::vector<double> *values;
};

class Line : public Primitive {
public:
	Line(double,double,double,double);
	virtual void draw() const;
	virtual ~Line(){};
	void setCoordinates(double,double,double,double);
	double get_x1();
	double get_x2();
	double get_y1();
	double get_y2();
private:
	double x1,x2,y1,y2;
};


#endif // GLWIDGET_H
