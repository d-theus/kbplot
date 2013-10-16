#include "kbplot.h"
#include <vector>

void KbPlot::drawAxis(){
	std::vector<double> *v = new std::vector<double>();
	v->push_back(-0.98);
	v->push_back(-0.98);

	v->push_back(-0.98);
	v->push_back(0.98);

	v->push_back(0.98);
	v->push_back(0.98);

	v->push_back(0.98);
	v->push_back(-0.98);

	v->push_back(-0.98);
	v->push_back(-0.98);

	qDebug()<<"Constructing polyline axis";
	Polyline *axis = new Polyline(v);
	axis->setTranslation(0.0, 0.0);
	qDebug()<<"Done";

	qDebug()<<"appending axis object to list";
	this->container->addObject(2, axis);
	qDebug()<<"Done";
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
