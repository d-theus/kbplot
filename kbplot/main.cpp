#include "glwidget.h"
#include "kbplot.h"
#include "txy.h"
#include <QApplication>
#include <QtGui>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    GLWidget w;
    KbPlot p(&w, -5.0, 5.0, -1.0, 10.0);
    std::vector<Txy> *v = new std::vector<Txy>();

    v->push_back(Txy(0.0,0.0));
    v->push_back(Txy(1.0,1.0));
    v->push_back(Txy(2.0,4.0));
    v->push_back(Txy(3.0,9.0));

    DataSet ds(v, DataSet::WITH_LINESPOINTS);
    p.addData(&ds);
    w.show();
    p.setRanges(-5.1, 5.0, -1.0, 10.0);
    p.draw();
    return a.exec();
}
