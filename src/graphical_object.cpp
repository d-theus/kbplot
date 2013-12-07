#include "graphical_object.h"
#include "txy.h"
#include "utilities.h"

#ifdef DEBUG
#include <QDebug>
#endif
const unsigned int maskr = 0xFF000000;
const unsigned int maskg = 0x00FF0000;
const unsigned int maskb = 0x0000FF00;
const unsigned int maska = 0x000000FF;

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
	
	glLineWidth(this -> style.lineThickness);
	if(this -> style.lineStroke != Style::LINE_STD){

		glEnable(GL_LINE_STIPPLE);
		glPushAttrib(GL_LINE_BIT);

		switch(style.lineStroke)
		{
			case Style::LINE_DASHED:
				glLineStipple(1, 0x00FF);
			break;
			case Style::LINE_DASH_DOTTED:
				glLineStipple(1, 0x0C0F);
			break;
			case Style::LINE_DOTTED:
				glLineStipple(1, 0xAAAA);
			break;
		}
	}
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
	if(this -> style.lineStroke != Style::LINE_STD){
		glPopAttrib();
		glDisable(GL_LINE_STIPPLE);
	}
}

Polyline::Polyline(std::vector<Txy> *data){
	this->data = data;
}

void Polyline::draw() const{
	if(this->data == NULL) return;
	this->data->size();

	before_draw();

	const unsigned int c = this -> style.lineColor;
	glColor4ub((c&maskr)>>24, (c&maskg)>>16, (c&maskb)>>8, c&maska);

	
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

	const unsigned int c = this -> style.lineColor;
	glColor4ub((c&maskr)>>24, (c&maskg)>>16, (c&maskb)>>8, c&maska);

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
	static const GLubyte textures[][8*8*1] = {{
		255,0,0,0,0,0,0,255,
		0,255,0,0,0,0,255,0,
		0,0,255,0,0,255,0,0,
		0,0,0,255,255,0,0,0,
		0,0,0,255,255,0,0,0,
		0,0,255,0,0,255,0,0,
		0,255,0,0,0,0,255,0,
		255,0,0,0,0,0,0,255
	},
	{
		255,255,255,255,255,255,255,255,
		255,255,255,255,255,255,255,255,
		255,255,255,255,255,255,255,255,
		255,255,255,255,255,255,255,255,
		255,255,255,255,255,255,255,255,
		255,255,255,255,255,255,255,255,
		255,255,255,255,255,255,255,255,
		255,255,255,255,255,255,255,255
	},
	{
		0,0,255,255,255,255,0,0,
		0,255,255,255,255,255,255,0,
		255,255,255,255,255,255,255,255,
		255,255,255,255,255,255,255,255,
		255,255,255,255,255,255,255,255,
		255,255,255,255,255,255,255,255,
		0,255,255,255,255,255,255,0,
		0,0,255,255,255,255,0,0
	},
	{
		0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,
		0  ,0  ,0  ,255,255,0  ,0  ,0  ,
		0  ,0  ,255,255,255,255,0  ,0  ,
		0  ,0  ,255,255,255,255,255,0  ,
		0  ,255,255,255,255,255,255,0  ,
		255,255,255,255,255,255,255,255,
		0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,
		0  ,0  ,0  ,0  ,0  ,0  ,0  ,0 
	}};

	float ms = style.markerSize;
	float sw = myglScreenWH().x;
	float sh = myglScreenWH().y;
	float pw = myglPlaneWH().x;
	float ph = myglPlaneWH().y;

	float rx = pw/sw;//how much is 1 pixel?
	float ry = ph/sh;

	GLfloat hside = rx*ms/2;
	GLfloat vside = ry*ms/2;
	
	GLfloat vertices[][2] = { {-hside,vside}, {hside,vside}, {hside,-vside}, {-hside,-vside}};

	before_draw();

	const unsigned int c = this -> style.markerColor;
	glColor4ub((c&maskr)>>24, (c&maskg)>>16, (c&maskb)>>8, c&maska);

	unsigned int tex = 0;
	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE0);
	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D,0,GL_LUMINANCE,8,8,0,GL_LUMINANCE,GL_UNSIGNED_BYTE,textures[this->style.markerType]);


	for(vector<Txy>::const_iterator
			i = data->begin(); i != data->end(); i++){

		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glTranslated(i->x, i->y, 0);

		glBegin(GL_QUADS);
		glTexCoord2f(0.0,0.0);
		glVertex2fv(vertices[0]);
		glTexCoord2f(1.0,0.0);
		glVertex2fv(vertices[1]);
		glTexCoord2f(1.0,1.0);
		glVertex2fv(vertices[2]);
		glTexCoord2f(0.0,1.0);
		glVertex2fv(vertices[3]);
		glEnd();

		glPopMatrix();
	}

	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);
	after_draw();
}

Text::Text(const string &text, size_t size, double x, double y){
	if(!(font = dtx_open_font("./res/Ubuntu-R.ttf", 32))){
		fprintf(stderr, "Unable to open font\n");
	}
	this -> x = x;
	this -> y = y;
	this -> text = text;
	this -> size = size;
	this -> setTranslation(x,y);
}

void Text::draw()const{
	//getting screen sizes in pixels
	GLint vp[4];//viewport
	glGetIntegerv(GL_VIEWPORT, vp);
	int w = vp[2], h = vp[3];

	before_draw();

	const unsigned int c = this -> style.textColor;
	glColor4ub((c&maskr)>>24, (c&maskg)>>16, (c&maskb)>>8, c&maska);

	dtx_prepare(this->font, 32);
	dtx_use_font(this->font,32);
	dtx_string(this->text.c_str());

	after_draw();
}

