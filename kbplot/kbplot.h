#include "glwidget.h"
#include "event.h"

struct DataSet {
		DataSet(std::vector<double> *data);
		virtual ~DataSet();

		bool wlines = false;
		bool wmarks = true;

		GraphicalObject *gobj[2];
};


class KbPlot : IMouseEventListener {
	public:
		static const double c_frameThickness = 0.02;
		static const double c_tickLength = 0.02;
		 
		KbPlot(GLWidget*, double, double, double, double);
		~KbPlot();
		void setRanges(double,double,double,double);
		void addData(DataSet &ds);
		void draw();

		virtual void mouseMoveEvent(int,int);
		virtual void mousePressEvent(int,int);
		virtual void mouseReleaseEvent(int,int);
		virtual void mouseScrollEvent(int);
	private:

		void drawAxis();
		void drawNumbers();
		void drawBg();

		Polyline *frame;
		std::vector<Line*> xticks_t;
		std::vector<Line*> xticks_b;
		std::vector<Line*> yticks_t;
		std::vector<Line*> yticks_b;
		vector<double> v1;

		GLWidget *container;

		double xmax, ymax, xmin, ymin;
		vector<DataSet> datasets;
};
