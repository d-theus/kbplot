#include "kbplot.h"

void KbPlot::drawAxis(){
}

void KbPlot::init(){
	drawAxis();
}

KbPlot::KbPlot(GLWidget *_container){
	this->container = _container;
	if(container != NULL){
		init();
	}
}
