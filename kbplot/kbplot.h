#include "glwidget.h"

class KbPlot {
	public:
		KbPlot(GLWidget *container);

		void init();
	private:
		void drawAxis(double, double, double, double);
		void drawNumbers();
		void drawBg();

		GLWidget *container;
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

