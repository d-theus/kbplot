#include "kbplot.h"

void KbPlot::drawAxis(){
	this->container->line(0,-200,0,200);
	this->container->line(-200,0,200,0);
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
