#include "glwidget.h"
#include "event.h"


class KbPlot : IMouseEventListener {
	public:
		KbPlot(GLWidget*, double, double, double, double);
		~KbPlot();
		void setRanges(double,double,double,double);

		int m_to_pix_x(double);
		int m_to_pix_y(double);

		double m_to_gl_x(double);
		double m_to_gl_y(double);

		double gl_to_m_x(double);
		double gl_to_m_y(double);

		double pix_to_m_x(int);
		double pix_to_m_y(int);

		virtual void mouseMoveEvent(int,int);
		virtual void mousePressEvent(int,int);
		virtual void mouseReleaseEvent(int,int);
	private:
		void drawAxis();
		void drawNumbers();
		void drawBg();

		Line *axis_xt, *axis_xb, *axis_yl, *axis_yr;
		std::vector<Line*> xticks;
		std::vector<Line*> yticks;

		GLWidget *container;

		double xmax, ymax, xmin, ymin;
};

class DataSet {
	public:
		DataSet (const std::vector<double> &data);
		void draw();

	private:
		bool wlines;
		bool wmarks;
};

class Style {
	public:
		Style ();
		virtual ~Style ();

		unsigned int color;
};

class LineStyle : public Style {
	public:
		LineStyle();
		unsigned int stroke;
		unsigned int thickness;
};

class MarkStyle : public Style {
	public:
		MarkStyle();
		unsigned int size;
};

