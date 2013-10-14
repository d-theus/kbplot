#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QGLWidget>
#include <QtOpenGL>
#include <list>

class Primitive;
class Polyline;

class GLWidget : public QGLWidget
{
	Q_OBJECT

	float xpos, ypos;
	int mouseX, mouseY;


public:
	std::list<Primitive*> objects;
    GLWidget(QWidget *parent = 0);
    void initializeGL();
    void resizeGL(int nw, int nh);
    void paintGL();

    void mouseMoveEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);

    inline float translateFromMathX(float x);
    inline float translateFromMathY(float y);
    void translateToMath();

};

class Primitive {
public:
	virtual ~Primitive(){};
	virtual void draw() const = 0;
};

class Polyline : public Primitive {
public:
	Polyline(std::vector<double> *);
	virtual void draw() const;
	virtual ~Polyline(){};
private:
	std::vector<double> *values;
};

#endif // GLWIDGET_H
