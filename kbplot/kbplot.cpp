#include "kbplot.h"
#include <vector>

void KbPlot::drawAxis(double xmin, double xmax, double ymin, double ymax){
	static Line 
		xt(-0.98, 0.98, 0.98, 0.98),
		xb(-0.98, -0.98, 0.98, -0.98),
		yl(-0.98, 0.98, -0.98, -0.98),
		yr(0.98, -0.98, 0.98, 0.98);

	xt.setTranslation(0.0,0.0);
	xb.setTranslation(0.0,0.0);
	yl.setTranslation(0.0,0.0);
	yr.setTranslation(0.0,0.0);

	container -> addObject("axis_xt", (Primitive*)&xt);
	container -> addObject("axis_xb", (Primitive*)&xb);
	container -> addObject("axis_yl", (Primitive*)&yl);
	container -> addObject("axis_yr", (Primitive*)&yr);

	static Line
		t1(-0.95, -0.98, -0.95, -0.97);

	double trX = container->xpos;
	double trY = container->ypos;
	t1.setTranslation(trX,trY);

	container -> addObject("test_tick", (Primitive*)&t1);
}

void KbPlot::init(){
	drawAxis(-1.0, 1.0, -1.0, 1.0);
}

KbPlot::KbPlot(GLWidget *_container, double _xmin, double _xmax, double _ymin, double _ymax){
	xmin = _xmin;
	xmax = _xmax;
	ymin = _ymin;
	ymax = _ymax;
	this->container = _container;
	if(container != NULL){
		init();
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
