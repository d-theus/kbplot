#include "glwidget.h"

#ifdef DEBUG
#include <QDebug>
#endif

GLWidget::GLWidget(QWidget *parent): QGLWidget(parent)
{
	setFormat(QGLFormat(QGL::SingleBuffer));
	initializeGL();
}

void GLWidget::initializeGL() {
}

void GLWidget::paintGL(){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();


	qDebug() << "Drawing objects";
	for(std::map<string, GraphicalObject*>::const_iterator i = this->objects.begin(); i != this->objects.end(); i++){
		i->second->draw();
	}
}

void GLWidget::resizeGL(int nw, int nh){
	glMatrixMode(GL_PROJECTION);
	glViewport(0,0, width(), height());
}

void GLWidget::mouseMoveEvent(QMouseEvent *e){
	for(std::vector<IMouseEventListener*>::const_iterator i = this->mouseListeners.begin(); i != mouseListeners.end(); i++){
		(*i)->mouseMoveEvent(e->x(), e->y());
	}
	updateGL();
}

void GLWidget::mouseReleaseEvent(QMouseEvent *e){
	for(std::vector<IMouseEventListener*>::const_iterator i = this->mouseListeners.begin(); i != mouseListeners.end(); i++){
		(*i)->mouseReleaseEvent(e->x(), e->y());
	}
	updateGL();
}

void GLWidget::addObject(string key, GraphicalObject *p){
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

void GLWidget::subscribeToMouse(IMouseEventListener*l){
	this->mouseListeners.push_back(l);
}

void GLWidget::setWorkingArea(double xmin, double xmax, double ymin, double ymax){
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(xmin, xmax, ymin, ymax, -1.0, 0.0);
}


