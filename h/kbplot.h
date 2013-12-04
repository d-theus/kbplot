#include "glwidget.h"
#include "event.h"
#include "txy.h"

using std::vector;

struct DataSet {
		DataSet(vector<Txy> *data);
		vector<Txy> *getData()const;
		virtual ~DataSet();
	private:
		vector<Txy> *data;
};


class KbPlot : IMouseEventListener {
	public:
		KbPlot(GLWidget*, double xmin, double xmax, double ymin, double ymax);
		KbPlot(GLWidget*);
		~KbPlot();

		void setRanges(double xmin, double xmax, double ymin, double ymax);

		void setBackground(const string filename);
		void setBackground(const unsigned int color);

		void setAxisTick(double);
		void setAxisXTick(double);
		void setAxisYTick(double);
		void setAxisXName(const string);
		void setAxisYName(const string);

		void setGridTick(const int);
		void setGridXTick(const int);
		void setGridYTick(const int);
		void setGridStroke(const Style::LineStroke)  ;
		void setGridXStroke(const Style::LineStroke) ;
		void setGridYStroke(const Style::LineStroke) ;

		void setGridThickness(const unsigned int);
		void setGridColor(const unsigned int);

		void draw(DataSet &ds, Style &s);
		void exportAsImage(const string filename);

		virtual void mouseMoveEvent(int x,int y);
		virtual void mousePressEvent(int x,int y);
		virtual void mouseReleaseEvent(int x,int y);
		virtual void mouseScrollEvent(int angle);
	private:
		static const double c_frameThickness = 0.02;
		static const double c_tickLength = 0.2;

		double axisAdjustMin(double min, double tick);//snap to powers of 10
		void axisRebuild(); //build new ticks collections



		Polyline *frame;
		std::vector<Line*> xticks_t;
		std::vector<Line*> xticks_b;
		std::vector<Line*> yticks_r;
		std::vector<Line*> yticks_l;
		vector<Txy> framePoints;

		GLWidget *container;
		bool mouseNewMovement = true;

		double xmax, ymax, xmin, ymin;
		double xtick, ytick;
		double gridx, gridy;

		vector<const DataSet*> datasets;

		Style stAxisX, stAxisY, stBkgnd, stGridX, stGridY;
};
