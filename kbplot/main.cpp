#include "glwidget.h"
#include "kbplot.h"
#include <QApplication>
#include <QtGui>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    GLWidget w;
    KbPlot p(&w, -5.0, 5.0, -1.0, 10.0);
    std::vector<double> *v = new std::vector<double>();
    v->push_back(0.0);
    v->push_back(0.0);

    v->push_back(1.0);
    v->push_back(1.0);

    v->push_back(2.0);
    v->push_back(4.0);

    v->push_back(3.0);
    v->push_back(9.0);
    DataSet ds(v, DataSet::WITH_POINTS);
    p.addData(ds);
    p.draw();
    w.show();
    return a.exec();
}
