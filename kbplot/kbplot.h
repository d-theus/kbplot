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
		KbPlot(GLWidget*, double xmin, double xmax, double ymin, double ymax);
		KbPlot(GLWidget*);
		~KbPlot();

		void setRanges(double xmin, double xmax, double ymin, double ymax);
		void addData(const DataSet *ds);

		void setGrid(bool);

		void setBackground(const string filename);
		void setBackground(const unsigned int color);

		void setXTick(double);
		void setYTick(double);
		void setGridTicksX(int);
		void setGridTicksY(int);

		void draw();
		void exportAsImage(const string filename);

		virtual void mouseMoveEvent(int x,int y);
		virtual void mousePressEvent(int x,int y);
		virtual void mouseReleaseEvent(int x,int y);
		virtual void mouseScrollEvent(int angle);

		Style gridStyle;
		bool gridEnabled;
	private:
		static const double c_frameThickness = 0.02;
		static const double c_tickLength = 0.2;

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
		double xtick, ytick;

		vector<const DataSet*> datasets;
};
