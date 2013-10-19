#include "glwidget.h"
#include "kbplot.h"
#include <QApplication>
#include <QtGui>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    GLWidget w;
    KbPlot p(&w, -1.0, 1.0, -1.0, 1.0);
    std::vector<double> *v = new std::vector<double>();
    v->push_back(0.1);
    v->push_back(0.1);
    v->push_back(0.4);
    v->push_back(0.5);
    Polyline pl(v);
    w.addObject("example",(Primitive*)&pl);
    w.show();
    return a.exec();
}
