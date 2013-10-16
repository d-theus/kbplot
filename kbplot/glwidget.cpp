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


	qDebug() << "Looping through objects";
	for(std::list<Primitive*>::const_iterator i = this->objects.begin(); i != this->objects.end(); i++){
		qDebug() << "Calling draw";
		(*i)->draw();
	}
	qDebug() << "Done looping";
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

inline float GLWidget::translateFromMathX(float x){
	return x/width();
}
inline float GLWidget::translateFromMathY(float y){
	return y/height();
}

void Primitive::setTranslation(double x, double y){
	this->isTranslated = true;
	this->trX = x;
	this->trY = y;
}

void Primitive::setTranslation(bool b){
	this->isTranslated = b;
}

Polyline::Polyline(std::vector<double> *vs){
	this->values = vs;
}

void Polyline::draw() const{
	qDebug()<<"We are in draw";
	if(this->values == NULL) return;
	qDebug()<<"Checking if values valid";
	this->values->size();

	//TODO: this is not DRY, refactor needed
	if(this->isTranslated){
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glLoadIdentity();
		glTranslated(trX, trY,0.0);
	}
	
	glBegin(GL_LINE_STRIP);
	for (std::vector<double>::const_iterator i = this->values->begin(); i != this->values->end() && i+1 != this->values->end(); i+=2) {
		qDebug() << "x:"; *i;
		qDebug() << "y:" ; *(i+1);
		glVertex2d(*i, *(i+1));
	}
	glEnd();

	if(this->isTranslated){
		glPopMatrix();
	}
}
