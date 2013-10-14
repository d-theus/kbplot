#include "glwidget.h"
#include "kbplot.h"
#include <QApplication>
#include <QtGui>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    GLWidget w;
    KbPlot p(&w);
	std::vector<double> *v = new std::vector<double>();
	v->push_back(0.1f);
	v->push_back(0.1f);
	v->push_back(0.2f);
	v->push_back(0.1f);
	v->push_back(0.1f);
	v->push_back(0.4f);
	Polyline pl(v);
	w.objects.push_back((Primitive*)(&pl));
    w.show();
    v->clear();
    return a.exec();
}
