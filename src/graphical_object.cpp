п»ї//---------------------------------------------------------------------------

#pragma hdrstop

#include "h/graphical_object.h"
#include "h/txy.h"
#include "h/utilities.h"
#include "h/gl_helper.h"
#include <algorithm>
#include <cassert>
#include <limits>
//---------------------------------------------------------------------------
#pragma package(smart_init)
//Маски для извлечения компонент цвета
//Компоненты извлекаются побайтно, RGBA
const unsigned int maskr = 0xFF000000;//R
const unsigned int maskg = 0x00FF0000;//G
const unsigned int maskb = 0x0000FF00;//B
const unsigned int maska = 0x000000FF;//A

static void setGLColor(const unsigned c)
{
    /*
     * Вспомогательная функция для установки цвета
     */
    glColor4ub((c & maskr) >> 24, (c & maskg) >> 16, (c & maskb) >> 8, c & maska);
}

static unsigned interpolateColor(const unsigned c1, const unsigned c2, const double v1, const double v2, const double v)
{
    /*
     * Используется для получения цвета для значения функции при градиентной заливке с учетом экстремумов.
     */

    //Значение должно находиться между максимумом и минимумом.
    assert(v1 <= v && v <= v2);

    const double k2 = (v - v1) / (v2 - v1), k1 = 1 - k2;
    return (unsigned) ((c1 & maskr) * k1 + (c2 & maskr) * k2) & maskr
           |  (unsigned) ((c1 & maskg) * k1 + (c2 & maskg) * k2) & maskg
           |  (unsigned) ((c1 & maskb) * k1 + (c2 & maskb) * k2) & maskb
           |  (unsigned) ((c1 & maska) * k1 + (c2 & maska) * k2) & maska;
}


Style::Style():
    //Умолчания:
    lineStroke(LINE_STD), //Сплошная
    lineColor(0xFFFFFFFF),//Белый
    lineThickness(1.0),

    markerType(MARK_CROSS),//Кресты
    markerSize(5),
    markerColor(0xFFFFFFFF),//Белый
    textAlignment(TEXT_ALIGN_CENTER),//Поверх точки(в центре)

    fontSize(16),
    fontName(""),
    textColor(0xFFFFFFFF),//Белый

    fillEnable(false),
    extremumFill(false),
    fillColor(0x00000000),//Черный
    fillColorMax(0x00000000)//Черный
{}

GraphicalObject::GraphicalObject(DataSet *data = NULL) :
    data(data), isFixed(false),  isScaled(false), isTranslated(false) { }

void GraphicalObject::setScale(double x, double y)
{
    this->isScaled = true;
    this->scX = x;
    this->scY = y;
}

void GraphicalObject::setTranslation(double x, double y)
{
    this -> isTranslated = true;
    this -> trX = x;
    this -> trY = y;
}

void GraphicalObject::toggleScaled(bool b)
{
    this->isScaled = b;
}

void GraphicalObject::toggleFixed(bool b)
{
    this -> isFixed = b;
}

void GraphicalObject::toggleTranslated(bool b)
{
    this -> isTranslated = b;
}

void GraphicalObject::before_draw()const
{
    if(isFixed)
    {
        glMatrixMode(GL_PROJECTION);
        glPushMatrix();
        glLoadIdentity();
    }
    if (isScaled || isTranslated)
    {
        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glLoadIdentity();
        if (isTranslated) glTranslated(trX, trY, 0.0);
        if (isScaled) glScaled(scX, scY, 1.0);
    }
    glLineWidth(this -> style.lineThickness);
    if(this -> style.lineStroke != Style::LINE_STD)
    {

        glEnable(GL_LINE_STIPPLE);
        glPushAttrib(GL_LINE_BIT);

        switch(style.lineStroke)
        {
        case Style::LINE_DASHED:
            glLineStipple(1, 0x00FF);//Двоичное 0000 0000 1111 1111 пробел пробел тире тире
            break;
        case Style::LINE_DASH_DOTTED:
            glLineStipple(1, 0x0C0F);//Двоичное 0000 1100 0000 1111 пробел точка пробел тире
            break;
        case Style::LINE_DOTTED:
            glLineStipple(1, 0xAAAA);//Двоичное 1000 1000 1000 1000 точка точка точка точка
            break;
        }
    }
}

void GraphicalObject::after_draw()const
{
    if (isFixed)
    {
        glMatrixMode(GL_PROJECTION);
        glPopMatrix();
    }
    if (isTranslated || isScaled)
    {
        glMatrixMode(GL_MODELVIEW);
        glPopMatrix();
    }
    if(this -> style.lineStroke != Style::LINE_STD)
    {
        glPopAttrib();
        glDisable(GL_LINE_STIPPLE);
    }
}

void Polylines::draw() const
{

    before_draw();

    if(style.fillEnable && data->size() >= 2)
    {
        double minY = std::numeric_limits<double>::max();
        double maxY = -minY;
        if(style.extremumFill)
        {
            for(data->rewind(); !data->atEnd(); data->advance())
            {
                const double y = data->current().y;
                minY = std::min(minY, y);
                maxY = std::max(maxY, y);
            }
        }
        else
        {
            setGLColor(style.fillColor);
        }

        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glBegin(GL_QUADS);
        data->rewind();
        Txy prevP = data->current();
        if(style.extremumFill)
            setGLColor(interpolateColor(style.fillColor, style.fillColorMax, minY, maxY, prevP.y));
        data->advance();
        do
        {
            const Txy p = data->current();
            glVertex2d(prevP.x, 0);
            glVertex2d(prevP.x, prevP.y);

            if(prevP.y * p.y < 0) // Special cross-zero case
            {
                const Txy v = Txy(p.x - prevP.x, p.y - prevP.y);
                const double x = p.x - p.y * v.x / v.y;
                if(style.extremumFill)
                    setGLColor(interpolateColor(style.fillColor, style.fillColorMax, minY, maxY, 0));
                for(int i = 0; i < 4; ++i)
                    glVertex2d(x, 0);
            }

            if(style.extremumFill)
                setGLColor(interpolateColor(style.fillColor, style.fillColorMax, minY, maxY, p.y));
            glVertex2d(p.x, p.y);
            glVertex2d(p.x, 0);
            prevP = p;
            data->advance();
        }
        while(!data->atEnd());
        glEnd();
    }

    setGLColor(style.lineColor);


    glBegin(GL_LINE_STRIP);
    for(data->rewind(); !data->atEnd(); data->advance())
    {
        glVertex2d(data->current().x,data->current().y);
    }
    glEnd();

    after_draw();
}

void Polygon::draw()const
{

    before_draw();

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glBegin(GL_POLYGON);

    for(data->rewind(); !data->atEnd(); data->advance())
    {
        glVertex2d(data->current().x,data->current().y);
    }

    glEnd();
    after_draw();
}

Line::Line(double _x1, double _y1, double _x2, double _y2): x1(_x1), x2(_x2), y1(_y1), y2(_y2) { }

void Line::setCoordinates(double _x1, double _y1, double _x2, double _y2)
{
    x1 = _x1;
    x2 = _x2;
    y1 = _y1;
    y2 = _y2;
}

void Line::draw() const
{
    before_draw();

    setGLColor(style.lineColor);

    glBegin(GL_LINE_STRIP);
    glVertex2d(x1,y1);
    glVertex2d(x2,y2);
    glEnd();

    after_draw();
}


/*------------------TextMarkerSet----------------------------------------------*/
TextMarkerSet::TextMarkerSet(DataSet *ds, map<size_t ,string> *tmarkers)
{
    this->coords = ds;
    this->tmarkers = tmarkers;

}
//------------------------------------------------------------------------------------------------------
void TextMarkerSet::draw() const
{
    before_draw();
    double dx = 0.0;
    double dy = 0.0;
    switch (style.textAlignment)
    {
    case Style::TEXT_ALIGN_TOP:
        dy =10.0;
        break;
    case Style::TEXT_ALIGN_BOTTOM:
        dy = -10.0;
        break;
    case Style::TEXT_ALIGN_LEFT:
        dx = -10.0;
        break;
    case Style::TEXT_ALIGN_RIGHT:
        dx = 10.5;
        break;
    case Style::TEXT_ALIGN_TOPRIGHT:
        dx = 10.5, dy = 10.5;
        break;
    case Style::TEXT_ALIGN_BOTLEFT:
        dx = -10.5, dy = -10.5;
        break;
    case Style::TEXT_ALIGN_BOTRIGHT:
        dx = 10.5, dy = -10.5;
        break;
    default:
        ;
    }
    //const unsigned int c = this -> style.textColor;
    //glColor4ub((c&maskr)>>24, (c&maskg)>>16, (c&maskb)>>8, c&maska);
    map<size_t,string>::const_iterator it = tmarkers->begin();
    for(; it!=tmarkers->end(); ++it)
    {
        if(it->first < coords->size())
        {
            Text text(it->second, this->style.fontSize,(*coords)[(it->first)].x + dx,(*coords)[(it->first)].y + dy);
            text.style = this->style;
            text.draw();
        }
    }


    after_draw();
}
/*------------------TextMarkerSet-----------------------------------------------*/

void MarkerSet::draw() const
{

    static const GLubyte textures[][8*8*1] = {{
            255,0,0,0,0,0,0,255,
            0,255,0,0,0,0,255,0,
            0,0,255,0,0,255,0,0,
            0,0,0,255,255,0,0,0,
            0,0,0,255,255,0,0,0,
            0,0,255,0,0,255,0,0,
            0,255,0,0,0,0,255,0,
            255,0,0,0,0,0,0,255
        },
        {
            255,255,255,255,255,255,255,255,
            255,255,255,255,255,255,255,255,
            255,255,255,255,255,255,255,255,
            255,255,255,255,255,255,255,255,
            255,255,255,255,255,255,255,255,
            255,255,255,255,255,255,255,255,
            255,255,255,255,255,255,255,255,
            255,255,255,255,255,255,255,255
        },
        {
            0,0,255,255,255,255,0,0,
            0,255,255,255,255,255,255,0,
            255,255,255,255,255,255,255,255,
            255,255,255,255,255,255,255,255,
            255,255,255,255,255,255,255,255,
            255,255,255,255,255,255,255,255,
            0,255,255,255,255,255,255,0,
            0,0,255,255,255,255,0,0
        },
        {
            0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,
            0  ,0  ,0  ,255,255,0  ,0  ,0  ,
            0  ,0  ,255,255,255,255,0  ,0  ,
            0  ,0  ,255,255,255,255,255,0  ,
            0  ,255,255,255,255,255,255,0  ,
            255,255,255,255,255,255,255,255,
            0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,
            0  ,0  ,0  ,0  ,0  ,0  ,0  ,0
        }
    };

    float ms = style.markerSize;
    float sw = myglScreenWH().x;
    float sh = myglScreenWH().y;
    float pw = myglPlaneWH().x;
    float ph = myglPlaneWH().y;

    float rx = pw/sw;//how much is 1 pixel?
    float ry = ph/sh;

    GLfloat hside = rx*ms/2;
    GLfloat vside = ry*ms/2;

    GLfloat vertices[][2] = { {-hside,vside}, {hside,vside}, {hside,-vside}, {-hside,-vside}};

    before_draw();

    setGLColor(style.markerColor & ~maska); //alpha is better to be zero anyway

    unsigned int tex = 0;
    glEnable(GL_BLEND);
    glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_TEXTURE_2D);
    //glActiveTexture(GL_TEXTURE0);РІ РєРѕРЅРєСЂРµС‚РЅС‹Р№ РјРѕРјРµРЅС‚ РІСЂ.РёСЃРїРѕР»СЊР·СѓРµС‚СЃСЏ С‚РѕР»СЊРєРѕ 1 С‚РµСЃС‚СѓСЂР°,РїРѕ СѓРјРѕР»С‡Р°РЅРёСЋ РІС‹Р±СЂР°РЅРѕ GL_TEXTURE0
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D,0,GL_LUMINANCE,8,8,0,GL_LUMINANCE,GL_UNSIGNED_BYTE,textures[this->style.markerType]);

    for(data->rewind(); !data->atEnd(); data->advance())
    {

        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glTranslated(data->current().x, data->current().y, 0);

        glBegin(GL_QUADS);
        glTexCoord2f(0.0,0.0);
        glVertex2fv(vertices[0]);
        glTexCoord2f(1.0,0.0);
        glVertex2fv(vertices[1]);
        glTexCoord2f(1.0,1.0);
        glVertex2fv(vertices[2]);
        glTexCoord2f(0.0,1.0);
        glVertex2fv(vertices[3]);
        glEnd();

        glPopMatrix();
    }

    glDisable(GL_TEXTURE_2D);
    glDisable(GL_BLEND);
    after_draw();
}

Text::Text(const string &text, size_t size, double x, double y)
{
    this -> x = x;
    this -> y = y;
    this -> text = text;
    this -> size = size;
    this -> setTranslation(x,y);
    HDC hDC = CreateCompatibleDC(NULL);
    bf = new BitmapFont(hDC);
    bf->BuildFont(this->size);
}

void Text::draw()const
{

    before_draw();

    setGLColor(style.textColor);

    glEnable(GL_BLEND);
    bf->Print(0, 0, text.c_str());
    glDisable(GL_BLEND);

    after_draw();
}

void Text::setCoordinates(double x, double y)
{
    this -> x = x;
    this -> y = y;
    this -> setTranslation(x,y);
}

void Text::setText(const string &text)
{
    this -> text = text;
}

void Text::setText(const char* text)
{
    this -> text.append(text);
}

