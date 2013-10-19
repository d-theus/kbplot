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
}

void KbPlot::init(){
	drawAxis(-1.0, 1.0, -1.0, 1.0);
}

KbPlot::KbPlot(GLWidget *_container){
	this->container = _container;
	if(container != NULL){
		init();
	}
}
