#include "kbplot.h"
#include <sstream>
#include <vector>
#include <cmath>


void KbPlot::drawAxis(){

}

KbPlot::KbPlot(GLWidget *_container, double _xmin, double _xmax, double _ymin, double _ymax){
	int i;

	for (i = 0; i < 21; i++) {
		xticks.push_back(new Line(0,0,1,1));
	}

	setRanges(_xmin, _xmax, _ymin, _ymax);
	axis_xt = new Line(-0.98, 0.98, 0.98, 0.98);
	axis_xb = new Line(-0.98, -0.98, 0.98, -0.98);
	axis_yl = new Line(-0.98, 0.98, -0.98, -0.98);
	axis_yr = new Line(0.98, -0.98, 0.98, 0.98);


	this->container = _container;
	if(container != NULL){

		container -> addObject("axis_xt", (Primitive*)axis_xt);
		container -> addObject("axis_xb", (Primitive*)axis_xb);
		container -> addObject("axis_yl", (Primitive*)axis_yl);
		container -> addObject("axis_yr", (Primitive*)axis_yr);

		axis_xt->setTranslation(0.0,0.0);
		axis_xb->setTranslation(0.0,0.0);
		axis_yl->setTranslation(0.0,0.0);
		axis_yr->setTranslation(0.0,0.0);

		i = 0;
		for(std::vector<Line*>::iterator it= xticks.begin(); it != xticks.end(); it++){
			std::stringstream id;
			id << "tick_x_" << i++;
			(*it)->setTranslation(0.0, 0.0);
			container -> addObject(id.str(), (Primitive*)(*it));
		}

		container->subscribeToMouse(this);
	}
}

KbPlot::~KbPlot(){
	for(std::vector<Line*>::iterator i = xticks.begin(); i != xticks.end(); i++){
		delete *i;
	}
}
void KbPlot::setRanges(double _xmin, double _xmax, double _ymin, double _ymax){
	xmin = _xmin;
	xmax = _xmax;
	ymin = _ymin;
	ymax = _ymax;

	double dx = pow(10, floor(log10((xmax - xmin)/2.0)) -1);
	double x = floor(xmin);


	for(std::vector<Line*>::iterator i = xticks.begin(); i != xticks.end(); i++){
		(*i)->setCoordinates(m_to_gl_x(x), -0.98, m_to_gl_x(x), -0.97);
		x+=dx;
	}
}

double KbPlot::pix_to_m_x(int x){
	double w = container->width();
	return xmin + (xmax - xmin)*(double)x/w;
}

double KbPlot::pix_to_m_y(int y){
	double h = container->height();
	return ymin + (ymax - ymin)*(double)y/h;
}

int KbPlot::m_to_pix_x(double x){
	return container->width() * (xmax - x)/(xmax - xmin);
}

int KbPlot::m_to_pix_y(double y){
	return container->width() * (ymax - y)/(ymax - ymin);
}

double KbPlot::m_to_gl_x(double x){
	return (xmax - x)/(xmax-xmin) - 1.0;
}

double KbPlot::m_to_gl_y(double y){
	return (ymax - y)/(ymax-ymin) - 1.0;
}

void KbPlot::mouseMoveEvent(int x, int y){
	static int px = x;
	qDebug()<<"We are in kbplot and we know about mouse";
	qDebug()<<"kbplot: x:" << x << " y: " << y;
	qDebug()<<"kbplot: pix_to_m_x:" << pix_to_m_x(x);
	double dx = (xmax-xmin)*(double)(px - x)/container->width();
	qDebug()<<"kbplot: dx:" << dx ;
	setRanges(xmin + dx, xmax + dx, ymin, ymax);
	drawAxis();
	px = x;
}

void KbPlot::mousePressEvent(int,int){
}

void KbPlot::mouseReleaseEvent(int,int){
	setRanges(-1.0, 1.0, -1.0, 1.0);
}
