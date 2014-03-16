//---------------------------------------------------------------------------

#ifndef kbplotH
#define kbplotH

#include "glwidget.h"
#include "event.h"
#include "txy.h"

using std::vector;

class KbPlot : IMouseEventListener {
	public:

		KbPlot(GLWidget*, double xmin, double xmax, double ymin, double ymax);
		KbPlot(GLWidget*) : mouseNewMovement(true) { }
		~KbPlot();

		void setRanges(double xmin, double xmax, double ymin, double ymax);

		void setBackground(const string filename);
		void setBackground(const unsigned int color);

		void setAxisTick(double);
		void setAxisXTick(double);
		void setAxisYTick(double);
		void setAxisXName(const string);
		void setAxisYName(const string);

		void toggleGrid(bool b);
		void setGridTick(const int);
		void setGridXTick(const int);
		void setGridYTick(const int);
		void setGridStroke(const Style::LineStroke)  ;
		void setGridXStroke(const Style::LineStroke) ;
		void setGridYStroke(const Style::LineStroke) ;

		void setGridThickness(const unsigned int);
		void setGridColor(const unsigned int);

		void draw(DataSet *ds);
		void draw(DataSet *ds, Style &s);
		void draw(DataSet *ds,std::map<size_t, std::string>  *tmarks);
		void draw(DataSet *ds,std::map<size_t, std::string>  *tmarks, Style &s);
		void exportAsImage(const string filename);

		virtual void mouseMoveEvent(int x,int y);
		virtual void mousePressedEvent(int x,int y);
		virtual void mouseReleaseEvent(int x,int y);
		virtual void mouseScrollEvent(int angle);
	private:

		static const double c_frameThickness;
		static const double c_tickLength;
		static const double c_fontSize;

		double axisAdjustMin(double min, double tick);//snap to powers of 10
		void axisRebuild(); //build new ticks collections

		void gridRebuild(); //build new ticks collections

		std::vector<Line*> xticks_t;
		std::vector<Line*> xticks_b;
		std::vector<Line*> yticks_r;
		std::vector<Line*> yticks_l;

		std::vector<Text*> xmrk_t, xmrk_b;
		std::vector<Text*> ymrk_l, ymrk_r;

		std::vector<Line*> xgrid;
		std::vector<Line*> ygrid;

		std::vector<Txy> *framePoints;
		DSV *framePointsDS;

		GLWidget *container;
		bool mouseNewMovement;
		bool mouseLButtonDown;  		// событие перемещения сработает только с зажатой ЛКМ
		double xmax, ymax, xmin, ymin;
		double xtick, ytick;
		double xgtick, ygtick;
		bool  isGridEnabled;

		size_t datasetCounter;

		Style stAxisX, stAxisY, stBkgnd, stGridX, stGridY;
};

#endif
