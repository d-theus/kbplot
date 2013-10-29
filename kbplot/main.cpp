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
    v->push_back(0.5);
    v->push_back(0.5);
    v->push_back(-0.5);
    v->push_back(0.5);
    v->push_back(-0.5);
    v->push_back(-0.5);
    v->push_back(0.5);
    v->push_back(-0.5);
    v->push_back(0.5);
    v->push_back(0.5);
    Polyline pl(v);
    Marker mrk(0,0,MRK_TYPE_DOT, 8);
    w.addObject("example",(Primitive*)&pl);
    w.addObject("example mrk",(Primitive*)&mrk);
    w.show();
    return a.exec();
}
