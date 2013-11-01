#include "kbplot.h"
#include <sstream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <functional>

DataSet::DataSet(vector<double> *data){
	this -> gobj[0] = (GraphicalObject*) new Polyline(data);
	this -> gobj[1] = (GraphicalObject*) new MarkerSet(data);


}

DataSet::~DataSet(){
	qDebug() << "Deleting dataset";
	delete(this -> gobj[0]);
	delete(this -> gobj[1]);
}


void KbPlot::drawAxis(){
}

KbPlot::KbPlot(GLWidget *_container, double _xmin, double _xmax, double _ymin, double _ymax){

	for (int i = 0; i < 31; i++) {
		xticks_t.push_back(new Line(0,0,1,1));
		xticks_b.push_back(new Line(0,0,1,1));
	}

	const double framePos = 1.0 - KbPlot::c_frameThickness;
	axis_xt = new Line(-framePos, framePos, framePos, framePos);
	axis_xb = new Line(-framePos, -framePos, framePos, -framePos);
	axis_yl = new Line(-framePos, framePos, -framePos, -framePos);
	axis_yr = new Line(framePos, -framePos, framePos, framePos);


	this->container = _container;
	if(container != NULL){

		container -> addObject("axis_xt", (GraphicalObject*)axis_xt);
		container -> addObject("axis_xb", (GraphicalObject*)axis_xb);
		container -> addObject("axis_yl", (GraphicalObject*)axis_yl);
		container -> addObject("axis_yr", (GraphicalObject*)axis_yr);

		axis_xt->setFixed();
		axis_xb->setFixed();
		axis_yl->setFixed();
		axis_yr->setFixed();

		for (int i = 0; i < xticks_t.size(); i++) {
			std::stringstream id1, id2;
			id1 << "tick_xt_" << i;
			id2 << "tick_xb_" << i;

			container -> addObject(id1.str(), (GraphicalObject*)(xticks_t[i]));
			container -> addObject(id2.str(), (GraphicalObject*)(xticks_b[i]));

			const double scale = 1.0 - KbPlot::c_frameThickness;
			xticks_t[i]->setScale(scale, scale);
			xticks_b[i]->setScale(scale, scale);
		}

		setRanges(_xmin, _xmax, _ymin, _ymax);
		container->subscribeToMouse(this);
	}
}

KbPlot::~KbPlot(){
	for(std::vector<Line*>::iterator i = xticks_t.begin(); i != xticks_t.end(); i++){
		delete *i;
	}
	for(std::vector<Line*>::iterator i = xticks_b.begin(); i != xticks_b.end(); i++){
		delete *i;
	}
}
void KbPlot::setRanges(double _xmin, double _xmax, double _ymin, double _ymax){
	xmin = _xmin;
	xmax = _xmax;
	ymin = _ymin;
	ymax = _ymax;

	double dx = 0.5;
	double tick_len = (ymax - ymin)/100.0; //TODO: mathToGl?
	double tick_offset = (ymax-ymin)/2 *KbPlot::c_tickLength;
	double x = floor(xmin);

	for (int i = 0; i < xticks_t.size(); i++) {
		xticks_t[i]->setCoordinates(x, ymax, x, ymax - tick_len);
		xticks_b[i]->setCoordinates(x, ymin, x, ymin + tick_len);
		x+=dx;
	}

	container->setWorkingArea(_xmin,_xmax,_ymin,_ymax);
}

void KbPlot::mouseMoveEvent(int x, int y){
	static int px = x;
	qDebug()<<"We are in kbplot and we know about mouse";
	qDebug()<<"kbplot: x:" << x << " y: " << y;
	double dx = (xmax-xmin)*(double)(px - x)/(double)container->width();
	qDebug()<<"kbplot: dx:" << dx ;
	qDebug()<<"Ranges: "<<xmin<<" "<<xmax<<" "<<ymin<<" "<<ymax;
	setRanges(xmin + dx, xmax + dx, ymin, ymax);
	drawAxis();
	px = x;
}

void KbPlot::mousePressEvent(int,int){
}

void KbPlot::mouseReleaseEvent(int,int){
	qDebug()<<"We are in kbplot and we know about mouse release";
}

void KbPlot::mouseScrollEvent(int a){
}

void KbPlot::addData(DataSet &ds){
	this->datasets.push_back(ds);
	ds.gobj[0]->setScale(0.98, 0.98);
	ds.gobj[1]->setScale(0.98, 0.98);
}

void KbPlot::draw(){
	qDebug() << "Kbplot::draw(){";
	for(std::vector<DataSet>::iterator i = 
			datasets.begin(); i != datasets.end(); i++){
		container->addObject("ds1_l", i->gobj[0]);
		container->addObject("ds1_p", i->gobj[1]);
	}
	qDebug() << "}";
}
