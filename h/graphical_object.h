//---------------------------------------------------------------------------

#ifndef GRAPHICAL_OBJECT_H
#define GRAPHICAL_OBJECT_H

#include <vector>
#include <map>
#include <string>
#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include "BitmapFont.h"
#include <iostream>

#include "txy.h"
#include "dataset.hpp"

using std::vector;
using std::map;
using std::string;
using std::cout;

struct Style
{
    /*
     * Структура, содержащая информацию о стилях отображения
     * графических объектов. Содержит данные сразу о всех параметрах для
     * всех объектов. Отдельные параметры используются конкретным объектом
     * по мере необходимости.
     */
    typedef enum
    {
        LINE_STD,
        LINE_DASHED,
        LINE_DOTTED,
        LINE_DASH_DOTTED
    } LineStroke;

    typedef enum
    {
        MARK_CROSS,
        MARK_SQUARE,
        MARK_DOT,
        MARK_TRIANGLE
    } MarkerType;

    typedef enum
    {
        TEXT_ALIGN_CENTER,
        TEXT_ALIGN_TOP,
        TEXT_ALIGN_BOTTOM,
        TEXT_ALIGN_LEFT,
        TEXT_ALIGN_RIGHT,
        TEXT_ALIGN_TOPLEFT,
        TEXT_ALIGN_TOPRIGHT,
        TEXT_ALIGN_BOTLEFT,
        TEXT_ALIGN_BOTRIGHT
    } TextAlignment;

    LineStroke lineStroke;
    unsigned int lineColor;
    float lineThickness;

    MarkerType markerType;
    unsigned int markerSize;
    unsigned int markerColor;

    TextAlignment textAlignment;
    size_t fontSize;
    string fontName;
    unsigned int textColor;

    bool fillEnable, extremumFill;
    unsigned int fillColor, fillColorMax;

    Style();
};

class GraphicalObject
{
    /*
     * Графический объект. Содержит в себе основные и общие для всех
     * объектов методы.
     */
protected:
    bool isFixed;
    bool isScaled;
    bool isTranslated;

    double trX, trY;
    double scX, scY;

    DataSet *data;

    //Вызывается внутри draw(). Применяет необходимые параметры перемещения или поворота
    void before_draw()const;

    //Отменяет действия before_draw() для корректного рисования других объектов
    void after_draw()const;
public:
    GraphicalObject(DataSet *data);
    virtual ~GraphicalObject() {};
    virtual void draw() const = 0;

    void setScale(double,double);
    void setTranslation(double,double);

    //Прикрепить объект к плоскости экрана [0,1]x[0,1]
    //Используется, чтобы рисовать статичные элементы, вроде рамки
    void toggleFixed(bool b = true);
    void toggleScaled(bool b = true);
    void toggleTranslated(bool b = true);

    Style style;
};

class Polylines : public GraphicalObject
{
    /*
     * Ломаная линия.
     */
public:
    Polylines(DataSet *data):
        GraphicalObject(data) {};
    virtual void draw() const;
    virtual ~Polylines() {};
};

class Line : public GraphicalObject
{
    /*
     * Отрезок. Создан, чтобы не создавать отдельно хранящиеся
     * наборы данных для Polyline
     */
public:
    Line(double,double,double,double);
    virtual void draw() const;
    virtual ~Line() {};
    void setCoordinates(double,double,double,double);
private:
    double x1,x2,y1,y2;
};

class Polygon : public GraphicalObject
{
    /*
     * Многоугольник c заливкой.
     */
public:
    Polygon(DataSet *data):
        GraphicalObject(data) {}
    virtual void draw() const;
    virtual ~Polygon() {};
};

typedef enum
{
    MRK_TYPE_DOT,
    MRK_TYPE_CROSS
} MarkerType;

class Text;

class MarkerSet : public GraphicalObject
{
    /*
     * Набор графических меток(маркеров).
     */
public:
    MarkerSet (DataSet *data):
        GraphicalObject(data) {}
    virtual ~MarkerSet () {};

    virtual void draw() const;

private:

    mutable vector<Txy> prevLocations;
    mutable vector<string> prevStrings;
    mutable vector<Text*> textObjects;

    void deleteTextObjects() const
    {
        for(size_t i = 0; i < textObjects.size(); ++i) delete textObjects[i];
    }
};

class Text : public GraphicalObject
{
public:

    Text(const string &text, size_t size, double x, double y);
    ~Text()
    {
        delete bf;
    }
    virtual void draw()const;
    void setCoordinates(double x, double y);
    void setText(const string &);
    void setText(const char* text);

private:
    double x, y;
    size_t size;
    string text;
    BitmapFont *bf;
};

class TextMarkerSet: public GraphicalObject
{
    /*
     * Набор текстовых меток
     */
public:
    TextMarkerSet (DataSet *ds, map<size_t,string> *tmarkers);

    virtual ~TextMarkerSet() { };

    virtual void draw() const;

private:
    DataSet *coords;
    map<size_t ,string >*tmarkers;

};
#endif //GRAPHICAL_OBJECT_H
