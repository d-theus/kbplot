#include "glwidget.h"
#include<QDebug>

GLWidget::GLWidget(QWidget *parent): QGLWidget(parent)
{
	setFormat(QGLFormat(QGL::SingleBuffer));
	xpos = 0.0;
	ypos = 0.0;
	initializeGL();
	qDebug() << "Initialized GL";
}

void GLWidget::initializeGL() {
	glMatrixMode(GL_PROJECTION);
	glOrtho(-1.0, 1.0, 1.0, -1.0, -1.0, 0.0);
	glLoadIdentity();
}

void GLWidget::paintGL(){
	qDebug() << "Painting GL";

	glMatrixMode(GL_PROJECTION);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(xpos, ypos, 0);

	for( auto fun : this->routine){
		fun();
	}
}

void GLWidget::resizeGL(int nw, int nh){
	glMatrixMode(GL_PROJECTION);
	glViewport(0,0, width(), height());
}

void GLWidget::mouseMoveEvent(QMouseEvent *e){
	qDebug() << "Debug mouse: " << e->x() << " " << e->y();
	qDebug() << "Xpos " << xpos;
	qDebug() << "Ypos " << ypos;
	double x = e->x() - width()/2;
	double y = e->y() - height()/2;
	double dx = x - mouseX;
	double dy = y - mouseY;

	xpos += 2*dx/width();
	ypos -= 2*dy/height();
	mouseX = x;
	mouseY = y;
	updateGL();
}

void GLWidget::mouseReleaseEvent(QMouseEvent *e){
	xpos = 0.0;
	ypos = 0.0;
	updateGL();
}

void GLWidget::line(int x1, int y1,int x2, int y2){
	glBegin(GL_LINE_STRIP);
	glVertex2f(
			this->translateFromMathX(x1),
			this->translateFromMathY(y1)
			);
	glVertex2f(
			this->translateFromMathX(x2),
			this->translateFromMathY(y2)
			);
	glEnd();
}

inline float GLWidget::translateFromMathX(float x){
	return x/width();
}
inline float GLWidget::translateFromMathY(float y){
	return y/height();
}

void GLWidget::routinePush(std::function<void(void)> f){
	this->routine.push_back(f);
}
