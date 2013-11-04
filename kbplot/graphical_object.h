#ifndef GRAPHICAL_OBJECT
#define GRAPHICAL_OBJECT

#include <vector>
#include <map>
#include <string>
#include <GL/gl.h>

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

	LineStroke lineStroke;
	unsigned int lineColor = 0xFFFFFFFF;
	float lineThickness = 1.0;

	MarkerType markerType = MARK_SQUARE;
	unsigned int markerSize = 5;
};

class GraphicalObject {
	protected:
		bool isFixed= false;
		bool isScaled = false;
		double trX, trY;
		double scX, scY;
		void before_draw()const;
		void after_draw()const;
	public:
		virtual ~GraphicalObject(){};
		virtual void draw() const = 0;
//TODO: put em together if possible
		void setScale(double,double);
		void setScale(bool);
		void setFixed(bool b = true);

		Style style;
};

class Polyline : public GraphicalObject {
	public:
		Polyline(vector<double> *);
		virtual void draw() const;
		virtual ~Polyline(){};
	private:
		vector<double> *values;
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
		Polygon(vector<double> *);
		virtual void draw() const;
		virtual ~Polygon(){};
	private:
		vector<double> *data;
};


typedef enum {
	MRK_TYPE_DOT,
	MRK_TYPE_CROSS
} MarkerType;

class MarkerSet : public GraphicalObject {
	public:
		MarkerSet (vector<double> *data);
		virtual ~MarkerSet (){};

		virtual void draw() const;

	private:
		vector<double> *data;
};


#endif
