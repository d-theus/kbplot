#include "glwidget.h"
#include "kbplot.h"
#include "txy.h"
#include <QApplication>
#include <QtGui>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    GLWidget w;
    KbPlot p(&w, -10.0, 50.0, -1.0, 50.0);
    std::vector<Txy> *v = new std::vector<Txy>();

    v->push_back(Txy(0.0,0.0));
    v->push_back(Txy(1.0,1.0));
    v->push_back(Txy(2.0,4.0));
    v->push_back(Txy(3.0,9.0));

    DataSet ds(v);

    double data[256];
    for (int i = 0; i < 128; i+=2) {
	    data[i] = i;
	    data[i+1] = log(i*i*i);
    }
    DataSet ds1(RawData::get((unsigned char*)(&data[2]),50, 16, 0, 8, RawData::TYPE_DOUBLE));
    Style s;
    Style s1;
    s.lineColor = 0x00FF00FF;
    s1.lineColor = 0x5555FFFF;
    s.lineThickness = 1.0;
    s1.lineThickness = 3.0;
    s.markerSize = 15;
    s.markerColor = 0xFF000055;
    s.markerType = Style::MARK_CROSS;
    s1.lineStroke = Style::LINE_DASHED;
    s.lineStroke = Style::LINE_DASH_DOTTED;

    p.toggleGrid(true);
    p.setGridColor(0x77777777);
    p.setGridYStroke(Style::LINE_DOTTED);
    w.show();

    p.draw(ds1, s1);
    p.draw(ds, s);
    return a.exec();
}
