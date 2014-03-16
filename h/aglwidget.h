//---------------------------------------------------------------------------

#ifndef AGLWIDGET_H
#define AGLWIDGET_H

#include "event.h"
#include "graphical_object.h"

class AGLWidget {
public:
	virtual ~AGLWidget(){};

	virtual void GLpaint()=0;
	virtual void GLresize(int nh,int nw)=0;
	virtual bool GLinit() = 0;
    virtual void GLupdate() = 0;

    virtual int width() = 0;
	virtual int height() = 0;

	virtual void subscribeToMouse(IMouseEventListener *subscriber)=0;

    virtual void mousePressedEvent(int xPos, int yPos)=0;
	virtual void mouseMoveEvent(int xPos, int yPos)=0;
	virtual void mouseReleaseEvent(int xPos, int yPos)=0;
	virtual void wheelEvent(int delta)=0;

	virtual double trScreenToGLx(int)=0;
	virtual double trScreenToGLy(int)=0;
	virtual int trGLToScreenx(double)=0;
	virtual int trGLToScreeny(double)=0;

	virtual void addObject(const string&, GraphicalObject*)=0;
	virtual void addLayer(const string&)=0;
	virtual void clearLayer(const string&)=0;
	virtual void clearScene()=0;
	virtual void removeLayer(const string&)=0;


	virtual void setWorkingArea(double xmin, double xmax, double ymin, double ymax)=0;

	virtual void exportAsImage(const wchar_t* filename)=0;
};

AGLWidget *CreateGLWidget();

#endif//AGLWIDGET_H
