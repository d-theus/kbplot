#include "kbplot.h"
#include <sstream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <functional>

DataSet::DataSet(vector<Txy> *data){
}

DataSet::~DataSet(){
	qDebug() << "Deleting dataset" << this << "{";
	qDebug() << "polyline" << polyline;
	if(withLines) delete polyline;
	qDebug() << "marker set" << markerset;
	if(withPoints) delete markerset;
	qDebug() << "}";
}


void KbPlot::drawAxis(){
}

KbPlot::KbPlot(GLWidget *_container, double _xmin, double _xmax, double _ymin, double _ymax){

	for (int i = 0; i < 31; i++) {
		xticks_t.push_back(new Line(0,0,1,1));
		xticks_b.push_back(new Line(0,0,1,1));
		yticks_l.push_back(new Line(0,0,1,1));
		yticks_r.push_back(new Line(0,0,1,1));
	}

	const double framePos = 1.0 - KbPlot::c_frameThickness;
	
	v1.push_back(Txy(-1.0, 1.0));
	v1.push_back(Txy(-1.0, -1.0));
	v1.push_back(Txy(1.0, -1.0));
	v1.push_back(Txy(1.0, 1.0));
	v1.push_back(Txy(-1.0, 1.0));

	frame = new Polyline(&v1);
	frame->style.lineColor = 0x444444FF;
	frame->style.lineThickness = 15.0;


	this->container = _container;
	if(container != NULL){

		container -> addObject("frame", (GraphicalObject*)frame);

		frame->toggleFixed();

		for (int i = 0; i < xticks_t.size(); i++) {
			std::stringstream id1, id2, id3, id4;
			id1 << "tick_xt_" << i;
			id2 << "tick_xb_" << i;
			id3 << "tick_yr_" << i;
			id4 << "tick_yl_" << i;

			container -> addObject(id1.str(), (GraphicalObject*)(xticks_t[i]));
			container -> addObject(id2.str(), (GraphicalObject*)(xticks_b[i]));
			container -> addObject(id3.str(), (GraphicalObject*)(yticks_r[i]));
			container -> addObject(id4.str(), (GraphicalObject*)(yticks_l[i]));
		}

		this->xtick = (xmax - xmin)/20.0;
		this->ytick = (ymax - ymin)/20.0;
		container->subscribeToMouse(this);
		setRanges(_xmin, _xmax, _ymin, _ymax);
	}
}

KbPlot::~KbPlot(){
	qDebug() << "Kbplot destructor{";
	for(std::vector<Line*>::iterator i = xticks_t.begin(); i != xticks_t.end(); i++){
		delete *i;
	}
	for(std::vector<Line*>::iterator i = xticks_b.begin(); i != xticks_b.end(); i++){
		delete *i;
	}
	for(std::vector<Line*>::iterator i = yticks_r.begin(); i != yticks_r.end(); i++){
		delete *i;
	}
	for(std::vector<Line*>::iterator i = yticks_l.begin(); i != yticks_l.end(); i++){
		delete *i;
	}
	qDebug() << "}";
}

void KbPlot::setRanges(double _xmin, double _xmax, double _ymin, double _ymax){
	xmin = _xmin;
	xmax = _xmax;
	ymin = _ymin;
	ymax = _ymax;

	double dx = 0.5;
	double dy = 0.5;
	double xtick_len = (ymax - ymin)/200.0; //TODO: mathToGl?
	double ytick_len = (ymax - ymin)/200.0; //TODO: mathToGl?
	double x = double(floor(xmin));
	double y = double(floor(ymin));

	for (int i = 0; i < xticks_t.size(); i++) {
		xticks_t[i]->setCoordinates(x, ymax, x, ymax - xtick_len);
		xticks_b[i]->setCoordinates(x, ymin, x, ymin + xtick_len);
		yticks_l[i]->setCoordinates(xmax, y, xmax - ytick_len, y);
		yticks_r[i]->setCoordinates(xmin, y, xmin + ytick_len, y);
		x+=dx;
		y+=dy;
	}

	container->setWorkingArea(_xmin,_xmax,_ymin,_ymax);
	container->GLpaint();
}

void KbPlot::setXTick(double xt){
	this -> xtick = xt;
}

void KbPlot::setYTick(double yt){
	this -> ytick = yt;
}

void KbPlot::mouseMoveEvent(int x, int y){
	static int px = x;
	static int py = y;
	double dx = (xmax-xmin)*(double)(px - x)/(double)container->width();
	double dy = (ymax-ymin)*(double)(y - py)/(double)container->height();
	setRanges(xmin + dx, xmax + dx, ymin + dy, ymax + dy);
	px = x;
	py = y;
}

void KbPlot::mousePressEvent(int,int){
}

void KbPlot::mouseReleaseEvent(int,int){
	qDebug()<<"We are in kbplot and we know about mouse release";
	container->GLpaint();
}

void KbPlot::mouseScrollEvent(int a){
	double xrange = xmax - xmin;
	double yrange = ymax - ymin;
	double xscale = xrange*10e-5 * (double)a;
	double yscale = yrange*10e-5 * (double)a;
	setRanges(xmin-xscale, xmax+xscale, ymin-yscale, ymax+yscale);
}

void KbPlot::addData(const DataSet *ds){
	this->datasets.push_back(ds);
}

void KbPlot::draw(){
	qDebug() << "Kbplot::draw(){";
	for(vector<const DataSet *>::iterator i = 
			datasets.begin(); i != datasets.end(); i++){
		if((*i)->withLines) container->addObject("ds1_l", (*i)->polyline);
		if((*i)->withPoints) container->addObject("ds1_p", (*i)->markerset);
	}
	container -> resizeGL(container->width(), container->height());
	container -> GLpaint();
	qDebug() << "}";
}
