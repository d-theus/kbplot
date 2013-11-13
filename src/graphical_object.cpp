#include "graphical_object.h"

#ifdef DEBUG
#include <QDebug>
#endif


Style::Style(){
}
void GraphicalObject::setScale(double x, double y){
	this->isScaled = true;
	this->scX = x;
	this->scY = y;
}

void GraphicalObject::setTranslation(double x, double y){
	this -> isTranslated = true;
	this -> trX = x;
	this -> trY = y;
}

void GraphicalObject::toggleScaled(bool b){
	this->isScaled = b;
}

void GraphicalObject::toggleFixed(bool b){
	this -> isFixed = b;
}

void GraphicalObject::toggleTranslated(bool b){
	this -> isTranslated = b;
}

void GraphicalObject::before_draw()const{
	if(isFixed){
		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity();
	}
	if (isScaled || isTranslated) {
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glLoadIdentity();
		if (isTranslated) glTranslated(trX, trY, 0.0);
		if (isScaled) glScaled(scX, scY, 1.0);
	}
	
	const unsigned int c = this -> style.lineColor;
	const unsigned int r = 0xFF000000;
	const unsigned int g = 0x00FF0000;
	const unsigned int b = 0x0000FF00;
	const unsigned int a = 0x000000FF;

	glColor4ub((c&r)>>24, (c&g)>>16, (c&b)>>8, c&a);
	glLineWidth(this -> style.lineThickness);
}

void GraphicalObject::after_draw()const{
	if (isFixed) {
		glMatrixMode(GL_PROJECTION);
		glPopMatrix();
	}
	if (isTranslated || isScaled){
		glMatrixMode(GL_MODELVIEW);
		glPopMatrix();
	}
}

Polyline::Polyline(std::vector<Txy> *data){
	this->data = data;
}

void Polyline::draw() const{
	if(this->data == NULL) return;
	this->data->size();

	before_draw();
	
	glBegin(GL_LINE_STRIP);
	for (vector<Txy>::const_iterator i = data->begin(); i != data->end(); i++) {
		glVertex2d(i->x, i->y);
	}
	glEnd();

	after_draw();
}

Polygon::Polygon(vector<Txy> *d){
	this -> data = d;
}

void Polygon::draw()const{
	qDebug() << "drawing polygon";
	before_draw();

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glBegin(GL_POLYGON);

	qDebug() << "iterating through data";
	for(vector<Txy>::const_iterator i = 
			data->begin(); i != data->end(); i++){
		qDebug() << "i";
		glVertex2d(i->x, i->y);
	}
	qDebug() << "iterating through data : DONE";
	
	glEnd();
	after_draw();
}

Line::Line(double _x1, double _y1, double _x2, double _y2): x1(_x1), x2(_x2), y1(_y1), y2(_y2) { } 

void Line::setCoordinates(double _x1, double _y1, double _x2, double _y2){
	x1 = _x1;
	x2 = _x2;
	y1 = _y1;
	y2 = _y2;
}

void Line::draw() const {
	before_draw();

	glBegin(GL_LINE_STRIP);
	glVertex2d(x1,y1);
	glVertex2d(x2,y2);
	glEnd();

	after_draw();
}

MarkerSet::MarkerSet(vector<Txy> *data){
	this -> data = data;
}

void MarkerSet::draw() const{
	if (data == NULL || data->size() == 0) return;

	before_draw();
	
	glPointSize(this -> style.markerSize);
	glBegin(GL_POINTS);
	for(vector<Txy>::const_iterator
			i = data->begin(); i != data->end(); i++){
		glVertex2d(i->x, i->y);
	}
	glEnd();

	after_draw();
}

Text::Text(const string &text, double size, double x, double y){
	if(!(font = dtx_open_font("./res/Ubuntu-R.ttf", 16))){
		fprintf(stderr, "Unable to open font\n");
	}
	this -> x = x;
	this -> y = y;
	this -> text = text;
	double scaleFactor = size / 500.0;
	this -> setTranslation(x,y);
	this -> setScale(scaleFactor, scaleFactor);
}

void Text::draw()const{
	before_draw();

	dtx_prepare(this->font, 24);
	dtx_use_font(this->font,24);
	dtx_string(this->text.c_str());

	after_draw();
}

