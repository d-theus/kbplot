п»ї//---------------------------------------------------------------------------
/*
 *TODO: make fixed string key parts constants
 *
 */
#pragma hdrstop

#include "kbplot.h"
#include "utilities.h"
#include "graphical_object.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

#include <sstream>
#include <vector>
#include <math.h>

const double KbPlot::c_tickLength = 0.2;
const double KbPlot::c_frameThickness = 10.0;
const double KbPlot::c_fontSize = 12;



KbPlot::KbPlot(GLWidget *_container, double _xmin, double _xmax, double _ymin, double _ymax) :
				mouseNewMovement(true), mouseLButtonDown(false) {//Г±ГЇГЁГ±Г®ГЄ ГЁГ­ГЁГ¶ГЁГ Г«ГЁГ§Г Г¶ГЁГЁ ГЄГ®Г­Г±ГІГ°ГіГЄГІГ®Г°Г 

	this->xtick = this->ytick = 1e-308;
	this->xgtick = this -> ygtick = 1e-308;
	isGridEnabled = false;
	datasetCounter = 0;


	const double framePos = 1.0 - KbPlot::c_frameThickness;

	framePoints = new std::vector<Txy>();
	framePoints->push_back(Txy(-1.0, 1.0));
	framePoints->push_back(Txy(-1.0, -1.0));
	framePoints->push_back(Txy(1.0, -1.0));
	framePoints->push_back(Txy(1.0, 1.0));
	framePoints->push_back(Txy(-1.0, 1.0));

	framePointsDS = new DSV(framePoints);
	Polylines *frame = new Polylines(framePointsDS);
	frame->style.lineColor = 0x444444FF;
	frame->style.lineThickness = 15.0;
	frame->toggleFixed();


	this->container = _container;
	if(container != NULL){
			container->addLayer("axis");
			container->addLayer("grid");
			container->addLayer("frame");
			container->addLayer("marks");
			container->addLayer("lines");
			container->addLayer("text");


			axisRebuild();
			setRanges(_xmin, _xmax, _ymin, _ymax);

			container -> subscribeToMouse(this);
			container -> addObject("frame", (GraphicalObject*)frame);
		}
}

KbPlot::~KbPlot(){
	delete this->framePoints;
}

void KbPlot::setRanges(double _xmin, double _xmax, double _ymin, double _ymax){
	xmin = _xmin;
	xmax = _xmax;
	ymin = _ymin;
	ymax = _ymax;

	if(xtick < 1e-300) xtick = pow(10.0,floor(log10((xmax - xmin)/20.0))+1);
	if(ytick < 1e-300) ytick = pow(10.0,floor(log10((ymax - ymin)/20.0))+1);

	if (xgtick < 1e-300) xgtick = xtick;
	if (ygtick < 1e-300) ygtick = ytick;

	double xtick_len = (ymax - ymin)/50.0; //TODO: mathToGl?
	double ytick_len = (xmax - xmin)/50.0; //TODO: mathToGl?
	double x = axisAdjustMin(xmin, xtick);
	double y = axisAdjustMin(ymin, ytick);

	axisRebuild();
	gridRebuild();

	char mk[15];
	double mkshift = xtick_len;

	for (unsigned int i = 0; i < xticks_t.size(); i++) {
		sprintf(mk, "%.1lf", x);//TODO: variable precision

		xticks_t[i]->setCoordinates(x, ymax, x, ymax - xtick_len);
		xticks_b[i]->setCoordinates(x, ymin, x, ymin + xtick_len);
		xmrk_t[i]->setText(mk);
		xmrk_t[i]->setCoordinates(x-strlen(mk)*0.5*mkshift,ymax - 1.3*mkshift);
		xmrk_b[i]->setText(mk);
		xmrk_b[i]->setCoordinates(x-strlen(mk)*0.5*mkshift,ymin + 1.3*mkshift);
		x+=xtick;
	}
	for (int i = 0; i < yticks_l.size(); i++) {
		sprintf(mk, "%.1lf", y);//TODO: variable precision
		yticks_l[i]->setCoordinates(xmax, y, xmax - ytick_len, y);
		yticks_r[i]->setCoordinates(xmin, y, xmin + ytick_len, y);
		ymrk_l[i]->setText(mk);
		ymrk_l[i]->setCoordinates(xmin+mkshift,y);
		ymrk_r[i]->setText(mk);
		ymrk_r[i]->setCoordinates(xmax-2*strlen(mk)*mkshift,y);
		y+=ytick;
	}

	x = axisAdjustMin(xmin, xtick);
	y = axisAdjustMin(ymin, ytick);
	for (unsigned int i = 0; i < xgrid.size(); i++) {
		xgrid[i]->setCoordinates(x, ymin, x, ymax);
		x += xgtick;
	}
	for (unsigned int i = 0; i < ygrid.size(); i++) {
		ygrid[i]->setCoordinates(xmax, y, xmin, y);
		y += ygtick;
	}

	container->setWorkingArea(_xmin,_xmax,_ymin,_ymax);
	container->GLpaint();
}

void KbPlot::mousePressedEvent(int,int){
	mouseNewMovement = true;
	mouseLButtonDown = true;
}
void KbPlot::mouseMoveEvent(int x, int y){
	if(mouseLButtonDown)
	{
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
}


void KbPlot::mouseReleaseEvent(int,int){
	mouseNewMovement = true;
	mouseLButtonDown = false;
}

void KbPlot::mouseScrollEvent(int a){
	double xrange = xmax - xmin;
	double yrange = ymax - ymin;
	double xscale = xrange*10e-5 * (double)a;
	double yscale = yrange*10e-5 * (double)a;
	setRanges(xmin-xscale, xmax+xscale, ymin-yscale, ymax+yscale);
	Txy t = myglPlaneWH();
}

void KbPlot::draw(DataSet *ds){
	Style s;
	draw(ds,s);
}

void KbPlot::draw(DataSet *ds, std::map<size_t, std::string> *tmarks){
	Style s;
	draw(ds,tmarks,s);
}

void KbPlot::draw(DataSet *ds, Style &s){
#ifdef DEBUG
	cout << "Kbplot::draw(){"<<"\n";
#endif
	if(s.lineThickness > 0) {
		GraphicalObject *pl = new Polylines(ds);
		pl->style = s;
		container->addObject("lines",pl);
	}
	if(s.markerSize > 0) {
		GraphicalObject *ms = new MarkerSet(ds);
		ms->style = s;
		container->addObject("marks",ms);
	}
	container -> setWorkingArea(xmin, xmax, ymin, ymax); //ensure
	container -> GLpaint();
	datasetCounter++;
		#ifdef DEBUG
	cout << "}"<<"\n";
		#endif
} /*------------------------TextMarkerSet-----------------------------------------*/

void KbPlot::draw(DataSet *ds, std::map<size_t, std::string>  *tmarks, Style &s) {
	TextMarkerSet *tms = new TextMarkerSet(ds, tmarks);
	container -> addObject("text",tms);
	draw(ds,s);
	container -> setWorkingArea(xmin, xmax, ymin, ymax); //ensure
	container -> GLpaint();
}
/*------------------------TextMarkerSet-----------------------------------------*/
double KbPlot::axisAdjustMin(double min, double tick){
	double pow10tick = floor(log10(tick));

	min /= pow(10, pow10tick);
	min = floor(min);
	min *= pow(10, pow10tick);

	return min;
}

void KbPlot::axisRebuild(){

	container->clearLayer("axis");

	xticks_t.clear();
	xticks_b.clear();
	yticks_l.clear();
	yticks_r.clear();

	xmrk_t.clear();
	xmrk_b.clear();
	ymrk_r.clear();
	ymrk_l.clear();

	if(xtick > 1e-300 && ytick > 1e-300) {
	int xticksCount = static_cast<int>((xmax - xmin) / xtick) + 10;
	int yticksCount = static_cast<int>((ymax - ymin) / ytick) + 10;

	for (int i = 0; i < xticksCount; i++) {
		xticks_t.push_back(new Line(0,0,1,1));
		xticks_b.push_back(new Line(0,0,1,1));
			xmrk_t.push_back(new Text("", 12, 0,0));
			xmrk_b.push_back(new Text("", 12, 0,0));
	}
	for (int i = 0; i < yticksCount; i++) {
		yticks_l.push_back(new Line(0,0,1,1));
		yticks_r.push_back(new Line(0,0,1,1));
			ymrk_l.push_back(new Text("", 12, 0,0));
			ymrk_r.push_back(new Text("", 12, 0,0));
	}
        }
	for (int i = 0; i < xticks_t.size(); i++) {

		container -> addObject("axis", (GraphicalObject*)(xticks_t[i]));
		container -> addObject("axis", (GraphicalObject*)(xticks_b[i]));
		container -> addObject("axis", (GraphicalObject*)(xmrk_b[i]));
		container -> addObject("axis", (GraphicalObject*)(xmrk_t[i]));

	}

	for (unsigned int i = 0; i < yticks_l.size(); i++) {

		container -> addObject("axis", (GraphicalObject*)(yticks_r[i]));
		container -> addObject("axis", (GraphicalObject*)(yticks_l[i]));
		container -> addObject("axis", (GraphicalObject*)(ymrk_l[i]));
		container -> addObject("axis", (GraphicalObject*)(ymrk_r[i]));
	}
}

void KbPlot::gridRebuild(){

	container->clearLayer("grid");

	xgrid.clear();
	ygrid.clear();

	if(!this->isGridEnabled && xgtick > 1e-308 && ygtick > 1e-308) return;

	int xticksCount = (xmax - xmin) / xgtick + 10;
	int yticksCount = (ymax - ymin) / ygtick + 10;

	for (int i = 0; i < xticksCount; i++) {
		Line *l = new Line(0,0,0,0);
                l->style = this->stGridX;
                xgrid.push_back(l);
		container -> addObject("grid", (GraphicalObject*)(l));
	}
	for (int i = 0; i < yticksCount; i++) {
		Line *l = new Line(0,0,0,0);
                l->style = this->stGridY;
                ygrid.push_back(l);
		container -> addObject("grid", (GraphicalObject*)(l));
	}
}

void KbPlot::setBackground(const unsigned int color){
        this->stBkgnd.fillColor = color;
        setRanges(xmin,xmax,ymin,ymax);
	const unsigned int r =(color&0xFF000000)>>24;
	const unsigned int g =(color&0x00FF0000)>>16;
	const unsigned int b =(color&0x0000FF00)>>8;
	const unsigned int a =(color&0x000000FF);
	glClearColor((float)r/255.0, (float)g/255.0, (float)b/255.0, (float)a/255.0);
}

void KbPlot::setAxisXTick(double t){
	this -> xtick = t;
	axisRebuild();
	setRanges(xmin, xmax, ymin, ymax);
}

void KbPlot::setAxisYTick(double t){
	this -> ytick = t;
	axisRebuild();
	setRanges(xmin, xmax, ymin, ymax);
}

void KbPlot::setAxisTick(double t){
	this-> xtick = t;
	this-> ytick = t;
	axisRebuild();
	setRanges(xmin, xmax, ymin, ymax);
}


void KbPlot::setGridColor(const unsigned int color){
        this->stGridX.lineColor = color;
        this->stGridY.lineColor = color;
        setRanges(xmin,xmax,ymin,ymax);
}
void KbPlot::setGridXTick(const int t){
	if (this -> xtick > 1e-308)
		this->xgtick = xtick / (double)t;
	axisRebuild();
	setRanges(xmin,xmax,ymin,ymax);
}
void KbPlot::setGridYTick(const int t){
	if (this -> ytick > 1e-308)
		this->ygtick = ytick / (double)t;
	axisRebuild();
	setRanges(xmin,xmax,ymin,ymax);
}
void KbPlot::setGridTick(const int t){
	if (this -> xtick > 1e-308)
		this->xgtick = xtick / (double)t;
	if (this -> ytick > 1e-308)
		this->ygtick = ytick / (double)t;
	axisRebuild();
	setRanges(xmin,xmax,ymin,ymax);
}
void KbPlot::setGridStroke(const Style::LineStroke st)  {
        this->stGridX.lineStroke = st;
        this->stGridY.lineStroke = st;
}
void KbPlot::setGridXStroke(const Style::LineStroke st) {
        this->stGridX.lineStroke = st;
}
void KbPlot::setGridYStroke(const Style::LineStroke st) {
		this->stGridY.lineStroke = st;
}
void KbPlot::toggleGrid(bool b){
        this->isGridEnabled = b;
        setRanges(xmin,xmax,ymin,ymax);//<=> refresh, TODO:fix it, GLupdate() is the way
}
