//---------------------------------------------------------------------------
#ifndef MODELPLOT_H
#define MODELPLOT_H
//---------------------------------------------------------------------------
#include "KbPlot.h"
#include "GLWidget.h"

class ModelPlot
{
public:
    ModelPlot(GLWidget* view);
    ~ModelPlot();

    void Clear();
public:
    void CreateOption1();
    void CreateOption2();
    void CreateOption3();
    void CreateOption4();
    void CreateOption5();
private:

    KbPlot *plot;
    unsigned char currentOption;
    GLWidget *gl;
    DataSet *dataset;
    Style *style;
    std::map<size_t,string>  marks;
    std::vector<Txy> points;
    double rawdata[256];
    RawData<double> *rd;

};
#endif//MODELPLOT_H
