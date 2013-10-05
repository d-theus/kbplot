#include "glwidget.h"
#include "kbplot.h"
#include <QApplication>
#include <QtGui>
#include<functional>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    GLWidget w;
    KbPlot p(&w);
    auto f = [&w](){w.line(100,100,200,200);};
    w.routinePush(f);

    w.show();
    return a.exec();
}
