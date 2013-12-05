#ifndef GRAPHICAL_OBJECT
#define GRAPHICAL_OBJECT

#include <vector>
#include <map>
#include <string>
#include <GL/gl.h>
#include <GL/glu.h>
#include <drawtext.h>
#include <QDebug>

#include "txy.h"

using std::vector;
using std::string;

struct Style {
	typedef enum {
		LINE_STD,
		LINE_DASHED,
		LINE_DOTTED,
		LINE_DASH_DOTTED
	} LineStroke;

	typedef enum {
		MARK_DOT,
		MARK_SQUARE,
		MARK_TRIANGLE,
		MARK_CROSS
	} MarkerType;

	typedef enum {
		TEXT_ALIGN_CENTER,
		TEXT_ALIGN_TOP,
		TEXT_ALIGN_BOTTOM,
		TEXT_ALIGN_LEFT,
		TEXT_ALIGN_RIGHT,
		TEXT_ALIGN_TOPLEFT,
		TEXT_ALIGN_TOPRIGHT,
		TEXT_ALIGN_BOTLEFT,
		TEXT_ALIGN_BOTRIGHT
	} TextAlignment;

	LineStroke lineStroke = LINE_STD;
	unsigned int lineColor = 0xFFFFFFFF;
	float lineThickness = 1.0;

	MarkerType markerType = MARK_SQUARE;
	unsigned int markerSize = 5;

	TextAlignment textAlignment = TEXT_ALIGN_CENTER;
	size_t fontSize = 16;
	string fontName = "Ubuntu-R.ttf";

	Style();
};

class GraphicalObject {
	protected:
		bool isFixed= false;
		bool isScaled = false;
		bool isTranslated = false;

		double trX, trY;
		double scX, scY;

		void before_draw()const;
		void after_draw()const;
	public:
		virtual ~GraphicalObject(){};
		virtual void draw() const = 0;

		void setScale(double,double);
		void setTranslation(double,double);

		void toggleFixed(bool b = true);
		void toggleScaled(bool b = true);
		void toggleTranslated(bool b = true);

		Style style;
};

class Polyline : public GraphicalObject {
	public:
		Polyline(vector<Txy> *data);
		virtual void draw() const;
		virtual ~Polyline(){qDebug() << "Destroying polyline";};
	private:
		vector<Txy> *data;
};

class Line : public GraphicalObject {
	public:
		Line(double,double,double,double);
		virtual void draw() const;
		virtual ~Line(){};
		void setCoordinates(double,double,double,double);
	private:
		double x1,x2,y1,y2;
};

class Polygon : public GraphicalObject {
	public:
		Polygon(vector<Txy> *);
		virtual void draw() const;
		virtual ~Polygon(){};
	private:
		vector<Txy> *data;
};


typedef enum {
	MRK_TYPE_DOT,
	MRK_TYPE_CROSS
} MarkerType;

class MarkerSet : public GraphicalObject {
	public:
		MarkerSet (vector<Txy> *data);
		virtual ~MarkerSet (){};

		virtual void draw() const;

	private:
		vector<Txy> *data;
};

class Text : public GraphicalObject {
	public:
		Text(const string &text, size_t size, double x, double y);
		~Text(){};
		virtual void draw()const;

	private:
		double x, y;
		size_t size;
		string text;
		struct dtx_font *font;
};


static const unsigned char markerTexes[][64] = {
{ 0,0,0,0,0,0,0,0, 0,0,0,1,1,0,0,0, 0,0,0,1,1,0,0,0, 0,0,1,1,1,1,0,0, 0,1,1,1,1,1,1,0, 0,1,1,1,1,1,1,0, 1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0 }
};
#endif
