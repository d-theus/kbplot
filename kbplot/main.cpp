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
    v->push_back(0.1);
    v->push_back(0.1);
    v->push_back(0.4);
    v->push_back(0.5);
    Polyline pl(v);
    w.addObject(1,(Primitive*)&pl);
    w.show();
    return a.exec();
}
