/*
 *TODO: make fixed string key parts constants
 *
 */

#include "kbplot.h"
#include <sstream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <functional>

DataSet::DataSet(vector<Txy> *data){
	this -> data = data;
}

DataSet::~DataSet(){
}

vector<Txy> * DataSet::getData()const{
	return this->data;
}

KbPlot::KbPlot(GLWidget *_container, double _xmin, double _xmax, double _ymin, double _ymax){

	xtick = ytick = 0;
	xgtick = ygtick = 0;


	const double framePos = 1.0 - KbPlot::c_frameThickness;
	
	framePoints.push_back(Txy(-1.0, 1.0));
	framePoints.push_back(Txy(-1.0, -1.0));
	framePoints.push_back(Txy(1.0, -1.0));
	framePoints.push_back(Txy(1.0, 1.0));
	framePoints.push_back(Txy(-1.0, 1.0));

	frame = new Polyline(&framePoints);
	frame->style.lineColor = 0x444444FF;
	frame->style.lineThickness = 15.0;
	frame->toggleFixed();


	this->container = _container;
	if(container != NULL){

		axisRebuild();

		this->xtick = (xmax - xmin)/20.0;
		this->ytick = (ymax - ymin)/20.0;
		container->subscribeToMouse(this);
		setRanges(_xmin, _xmax, _ymin, _ymax);
	}
}

KbPlot::~KbPlot(){
}

void KbPlot::setRanges(double _xmin, double _xmax, double _ymin, double _ymax){
	xmin = _xmin;
	xmax = _xmax;
	ymin = _ymin;
	ymax = _ymax;

	if(xtick < 1e-300) xtick = (xmax - xmin)/20.0;
	if(ytick < 1e-300) ytick = (ymax - ymin)/20.0;

	if(xgtick < 1e-300) xgtick = xtick;
	if(ygtick < 1e-300) ygtick = ytick;

	double xtick_len = (ymax - ymin)/200.0; //TODO: mathToGl?
	double ytick_len = (xmax - xmin)/200.0; //TODO: mathToGl?
	double x = axisAdjustMin(xmin, xtick);
	double y = axisAdjustMin(ymin, ytick);

	container -> addObject("frame", (GraphicalObject*)frame);

	axisRebuild();
	gridRebuild();

	for (int i = 0; i < xticks_t.size(); i++) {
		xticks_t[i]->setCoordinates(x, ymax, x, ymax - xtick_len);
		xticks_b[i]->setCoordinates(x, ymin, x, ymin + xtick_len);
		x+=xtick;
	}
	for (int i = 0; i < yticks_l.size(); i++) {
		yticks_l[i]->setCoordinates(xmax, y, xmax - ytick_len, y); 
		yticks_r[i]->setCoordinates(xmin, y, xmin + ytick_len, y);
		y+=ytick;
	}

	x = axisAdjustMin(xmin, xtick);
	y = axisAdjustMin(ymin, ytick);
	for (int i = 0; i < xgrid.size(); i++) {
		xgrid[i]->setCoordinates(x, ymin, x, ymax);
		x += xgtick;
	}
	for (int i = 0; i < ygrid.size(); i++) {
		ygrid[i]->setCoordinates(xmax, y, xmin, y);
		y += ygtick;
	}

	container->setWorkingArea(_xmin,_xmax,_ymin,_ymax);
	container->GLpaint();
}

void KbPlot::mousePressEvent(int x, int y){
	mouseNewMovement = true;
}

void KbPlot::mouseMoveEvent(int x, int y){
	static int px = x;
	static int py = y;
	if(mouseNewMovement) {px = x; py = y;} //Avoid long jumps when release point
	mouseNewMovement = false;//		is far from new press point
	double dx = (xmax-xmin)*(double)(px - x)/(double)container->width();
	double dy = (ymax-ymin)*(double)(y - py)/(double)container->height();
	setRanges(xmin + dx, xmax + dx, ymin + dy, ymax + dy);
	px = x;
	py = y;
}

void KbPlot::mouseReleaseEvent(int,int){
	mouseNewMovement = true;
}

void KbPlot::mouseScrollEvent(int a){
	double xrange = xmax - xmin;
	double yrange = ymax - ymin;
	double xscale = xrange*10e-5 * (double)a;
	double yscale = yrange*10e-5 * (double)a;
	setRanges(xmin-xscale, xmax+xscale, ymin-yscale, ymax+yscale);
}

void KbPlot::draw(DataSet &ds, Style &s){
	qDebug() << "Kbplot::draw(){";
	if(s.lineThickness > 0) 
		container->addObject("ds1_l",(GraphicalObject*)(new Polyline(ds.getData())));
	if(s.markerSize > 0) 
		container->addObject("ds1_p",(GraphicalObject*)(new MarkerSet(ds.getData())));
	container -> setWorkingArea(xmin, xmax, ymin, ymax); //ensure
	container -> GLpaint();
	qDebug() << "}";
}

double KbPlot::axisAdjustMin(double min, double tick){
	double pow10tick = floor(log10(tick));

	min /= pow(10, pow10tick);
	min = floor(min);
	min *= pow(10, pow10tick);

	return min;
}

void KbPlot::axisRebuild(){
	for(vector<Line*>::iterator i = 
			xticks_t.begin(); i != xticks_t.end(); i++){
		delete(*i);
	}
	for(vector<Line*>::iterator i = 
			xticks_b.begin(); i != xticks_b.end(); i++){
		delete(*i);
	}
	for(vector<Line*>::iterator i = 
			yticks_r.begin(); i != yticks_r.end(); i++){
		delete(*i);
	}
	for(vector<Line*>::iterator i = 
			yticks_l.begin(); i != yticks_l.end(); i++){
		delete(*i);
	}
	container->removeWithPrefix("tick");

	xticks_t.clear();
	xticks_b.clear();
	yticks_l.clear();
	yticks_r.clear();

	int xticksCount = (xmax - xmin) / xtick + 10;
	int yticksCount = (ymax - ymin) / ytick + 10;

	for (int i = 0; i < xticksCount; i++) {
		xticks_t.push_back(new Line(0,0,1,1));
		xticks_b.push_back(new Line(0,0,1,1));
	}
	for (int i = 0; i < yticksCount; i++) {
		yticks_l.push_back(new Line(0,0,1,1));
		yticks_r.push_back(new Line(0,0,1,1));
	}

	for (int i = 0; i < xticks_t.size(); i++) {
		std::stringstream id1, id2;
		id1 << "tick_xt_" << i;
		id2 << "tick_xb_" << i;

		container -> addObject(id1.str(), (GraphicalObject*)(xticks_t[i]));
		container -> addObject(id2.str(), (GraphicalObject*)(xticks_b[i]));
	}

	for (int i = 0; i < yticks_l.size(); i++) {
		std::stringstream id1, id2;
		id1 << "tick_yr_" << i;
		id2 << "tick_yl_" << i;

		container -> addObject(id1.str(), (GraphicalObject*)(yticks_r[i]));
		container -> addObject(id2.str(), (GraphicalObject*)(yticks_l[i]));
	}
}

void KbPlot::gridRebuild(){
	for(std::vector<Line*>::iterator i = 
			xgrid.begin(); i != xgrid.end(); i++){
		delete(*i);
	}
	for(std::vector<Line*>::iterator i = 
			ygrid.begin(); i != ygrid.end(); i++){
		delete(*i);
	}
	

	container->removeWithPrefix("grid");

	xgrid.clear();
	ygrid.clear();

	int xticksCount = (xmax - xmin) / xgtick + 10;
	int yticksCount = (ymax - ymin) / ygtick + 10;

	for (int i = 0; i < xticksCount; i++) {
		std::stringstream id;
		xgrid.push_back(new Line(0,0,0,0));
		id << "grid_x_" << i;
		container -> addObject(id.str(), (GraphicalObject*)(xgrid[i]));
	}
	for (int i = 0; i < yticksCount; i++) {
		std::stringstream id;
		ygrid.push_back(new Line(0,0,0,0));
		id << "grid_y_" << i;
		container -> addObject(id.str(), (GraphicalObject*)(ygrid[i]));
	}
}

