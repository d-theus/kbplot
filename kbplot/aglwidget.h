#ifndef AGLWIDGET_H
#define AGLWIDGET_H

#include "event.h"
#include "graphical_object.h"

class AGLWidget {
public:
	virtual ~AGLWidget(){};

	virtual void GLpaint()=0;
	virtual void GLresize(int nh,int nw)=0;
	virtual void GLinitialize()=0;

	virtual void subscribeToMouse(IMouseEventListener *subscriber)=0;

	virtual double trScreenToGLx(int)=0;
	virtual double trScreenToGLy(int)=0;
	virtual int trGLToScreenx(double)=0;
	virtual int trGLToScreeny(double)=0;

	virtual void addObject(string, GraphicalObject*)=0;
	virtual void clearScene()=0;


	virtual void setWorkingArea(double xmin, double xmax, double ymin, double ymax)=0;
};

#endif

