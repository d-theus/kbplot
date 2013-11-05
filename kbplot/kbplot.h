#include "glwidget.h"
#include "event.h"
#include "txy.h"

using std::vector;

struct DataSet {
	typedef enum { WITH_LINES, WITH_POINTS, WITH_LINESPOINTS } LinesPointsEnabler;
		DataSet(vector<Txy> *data, LinesPointsEnabler lp = WITH_LINES);
		DataSet(const DataSet &){};
		virtual ~DataSet();

		bool withLines;
		bool withPoints;

		GraphicalObject *polyline;
		GraphicalObject *markerset;
};


class KbPlot : IMouseEventListener {
	public:
		KbPlot(GLWidget*, double, double, double, double);
		KbPlot(GLWidget*);
		~KbPlot();

		void setRanges(double,double,double,double);
		void addData(const DataSet *ds);

		void setGrid(bool);

		void setBackground(const string filename);
		void setBackground(const unsigned int color);

		void setXTick(double);
		void setYTick(double);

		void draw();
		void exportAsImage(const string filename);

		virtual void mouseMoveEvent(int,int);
		virtual void mousePressEvent(int,int);
		virtual void mouseReleaseEvent(int,int);
		virtual void mouseScrollEvent(int);

		Style gridStyle;
		bool gridEnabled;
	private:
		static const double c_frameThickness = 0.02;
		static const double c_tickLength = 0.02;

		void drawAxis();
		void drawNumbers();
		void drawBg();

		Polyline *frame;
		std::vector<Line*> xticks_t;
		std::vector<Line*> xticks_b;
		std::vector<Line*> yticks_r;
		std::vector<Line*> yticks_l;
		vector<Txy> v1;

		GLWidget *container;

		double xmax, ymax, xmin, ymin;
		vector<const DataSet*> datasets;
};
