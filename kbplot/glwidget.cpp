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
	for(std::map<string, Primitive*>::const_iterator i = this->objects.begin(); i != this->objects.end(); i++){
		qDebug() << "Calling draw";
		(*i).second->draw();
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

void GLWidget::addObject(string key, Primitive *p){
	this->objects[key] = p;
}

double GLWidget::scr_to_gl_x(int x){
	return (double)(this->width() - x)/this->width();
}

double GLWidget::scr_to_gl_y(int y){
	return (double)(this->height() - y)/this->height();
}

int GLWidget::gl_to_scr_x(double x){
	return (double)(this->width()) * (x - 1.0);
}

int GLWidget::gl_to_scr_y(double y){
	return (double)(this->width()) * (y - 1.0);
}

void Primitive::setTranslation(double x, double y){
	this->isTranslated = true;
	this->trX = x;
	this->trY = y;
}

void Primitive::setTranslation(bool b){
	this->isTranslated = b;
}

void Primitive::before_draw()const{
	if(isTranslated){
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glLoadIdentity();
		glTranslated(trX, trY,0.0);
	}
}

void Primitive::after_draw()const{
	if(isTranslated){
		glPopMatrix();
	}
}

Polyline::Polyline(std::vector<double> *vs){
	this->values = vs;
}

void Polyline::draw() const{
	qDebug()<<"We are in draw";
	if(this->values == NULL) return;
	qDebug()<<"Checking if values valid";
	this->values->size();

	before_draw();
	
	glBegin(GL_LINE_STRIP);
	for (std::vector<double>::const_iterator i = this->values->begin(); i != this->values->end() && i+1 != this->values->end(); i+=2) {
		qDebug() << "x:"; *i;
		qDebug() << "y:" ; *(i+1);
		glVertex2d(*i, *(i+1));
	}
	glEnd();

	after_draw();
}

Line::Line(double _x1, double _y1, double _x2, double _y2): x1(_x1), x2(_x2), y1(_y1), y2(_y2) { } 

void Line::setCoordinates(double _x1, double _y1, double _x2, double _y2){
	x1 = _x1;
	x2 = _x2;
	y1 = _y1;
	y2 = _y2;
}

void Line::draw() const {
	before_draw();

	glBegin(GL_LINE_STRIP);
	glVertex2d(x1,y1);
	glVertex2d(x2,y2);
	glEnd();

	after_draw();
}
