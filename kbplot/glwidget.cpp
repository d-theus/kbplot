#include "glwidget.h"

#ifdef DEBUG
#include <QDebug>
#endif

GLWidget::GLWidget(): QGLWidget()
{
	setFormat(QGLFormat(QGL::SingleBuffer));
	initializeGL();
}

void GLWidget::GLpaint(){
	this -> paintGL();
}
void GLWidget::GLinitialize(){
	this -> initializeGL();
}
void GLWidget::GLresize(int nw, int nh){
	this -> resizeGL(nw,nh);
}
void GLWidget::GLupdate(){
	this -> updateGL();
}

void GLWidget::initializeGL() {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glViewport(0,0, width(), height());
}

void GLWidget::paintGL(){
	qDebug() << "paintGL()";
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	for(std::map<string, GraphicalObject*>::const_iterator i = this->objects.begin(); i != this->objects.end(); i++){
		i->second->draw();
	}
	glFlush();
}

void GLWidget::resizeGL(int nw, int nh){
	qDebug() << "resizeGL()";
	glViewport(0,0, width(), height());
}

void GLWidget::mouseMoveEvent(QMouseEvent *e){
	for(std::vector<IMouseEventListener*>::const_iterator i = this->mouseListeners.begin(); i != mouseListeners.end(); i++){
		(*i)->mouseMoveEvent(e->x(), e->y());
	}
}

void GLWidget::mouseReleaseEvent(QMouseEvent *e){
	for(std::vector<IMouseEventListener*>::const_iterator i = this->mouseListeners.begin(); i != mouseListeners.end(); i++){
		(*i)->mouseReleaseEvent(e->x(), e->y());
	}
}
void GLWidget::wheelEvent(QWheelEvent *e){
	for(std::vector<IMouseEventListener*>::const_iterator i = this->mouseListeners.begin(); i != mouseListeners.end(); i++){
		(*i)->mouseScrollEvent(e->delta());
	}
}

void GLWidget::addObject(string key, GraphicalObject *p){
	this->objects[key] = p;
}

double GLWidget::trScreenToGLx(int x){
	return (double)(this->width() - x)/this->width();
}

double GLWidget::trScreenToGLy(int y){
	return (double)(this->height() - y)/this->height();
}

int GLWidget::trGLToScreenx(double x){
	return (double)(this->width()) * (x - 1.0);
}

int GLWidget::trGLToScreeny(double y){
	return (double)(this->width()) * (y - 1.0);
}

void GLWidget::subscribeToMouse(IMouseEventListener*l){
	this->mouseListeners.push_back(l);
}

void GLWidget::setWorkingArea(double xmin, double xmax, double ymin, double ymax){
	qDebug()<<"setWorkingArea("<<xmin<<","<<xmax<<","<<ymin<<","<<ymax<<")";
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(xmin, xmax, ymin, ymax, -1.0, 0.0);
}
