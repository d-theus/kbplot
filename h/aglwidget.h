//---------------------------------------------------------------------------

#ifndef AGLWIDGET_H
#define AGLWIDGET_H

#include "event.h"
#include "graphical_object.h"

class AGLWidget
{
    /*
     * Абстрактный класс, содержащий базовые методы работы с графической областью.
     */
public:
    virtual ~AGLWidget() {};

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

    //Трансляция из пиксельных координат в openGL [0,1]x[0,1]
    virtual double trScreenToGLx(int)=0;
    virtual double trScreenToGLy(int)=0;
    //И наоборот
    virtual int trGLToScreenx(double)=0;
    virtual int trGLToScreeny(double)=0;

    //Добавить объект к слою
    virtual void addObject(const string&, GraphicalObject*)=0;
    //Добавить именованный слой
    virtual void addLayer(const string&)=0;
    virtual void clearLayer(const string&)=0;
    virtual void removeLayer(const string&)=0;
    //Удалить со сцены все объекты
    virtual void clearScene()=0;


    //Установить границы отображаемой области плоскости ХОУ
    virtual void setWorkingArea(double xmin, double xmax, double ymin, double ymax)=0;

    virtual void exportAsImage(const wchar_t* filename)=0;
};

AGLWidget *CreateGLWidget();

#endif//AGLWIDGET_H
