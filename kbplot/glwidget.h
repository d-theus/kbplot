#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QGLWidget>
#include <QtOpenGL>
#include <functional>
#include <list>

class GLWidget : public QGLWidget
{
	Q_OBJECT


	float xpos, ypos;
	int mouseX, mouseY;
	std::list<std::function<void(void)> > routine;


public:
    GLWidget(QWidget *parent = 0);
    void initializeGL();
    void resizeGL(int nw, int nh);
    void paintGL();

    void mouseMoveEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);

    inline float translateFromMathX(float x);
    inline float translateFromMathY(float y);
    void translateToMath();

    void line(int x1, int y1, int x2, int y2);
    void text(int x, int y, std::string text);

    void routinePush(std::function<void(void)>);
};

#endif // GLWIDGET_H
