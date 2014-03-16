//---------------------------------------------------------------------------

#pragma hdrstop

#include "h/ModelPlot.h"
#include <Math.hpp>
//---------------------------------------------------------------------------
#pragma package(smart_init)

ModelPlot::ModelPlot(GLWidget* view) : gl(view),currentOption(0), plot(0), dataset(0), style(0), rd(0)
{}

ModelPlot::~ModelPlot()
{
    Clear();
}

void ModelPlot::Clear()
{
    if(points.size() > 0)
        points.clear();
    if(rd)
    {
        delete rd;
        rd = 0;
    }
    if(dataset)
    {
        delete dataset;
        dataset = 0;
    }
    if(style)
    {
        delete style;
        style = 0;
    }
    if(plot)
    {
        delete plot;
        plot = 0;
    }
    if(marks.size() > 0)
        marks.clear();
    currentOption = 0;
}

void ModelPlot::CreateOption1()
{
    if(currentOption == 1) return ;
    Clear();

    plot = new KbPlot(gl, -10.0, 50.0, -1.0, 50.0);

    points.push_back(Txy(0.0,0.0));
    points.push_back(Txy(1.0,1.0));
    points.push_back(Txy(-1.0,1.0));
    points.push_back(Txy(2.0,4.0));
    points.push_back(Txy(-2.0,4.0));
    points.push_back(Txy(3.0,9.0));
    points.push_back(Txy(-3.0,9.0));

    dataset = dynamic_cast<DataSet*>(new DSV(&points));

    style = new Style();
    style->lineColor = 0x00FF00FF;
    style->lineThickness = 1.0;
    style->markerSize = 20;
    style->markerColor = 0xFF000055;
    style->markerType = Style::MARK_TRIANGLE;
    style->fontSize = 24;
    style->textColor = 0xFF00FF00;
    style->textAlignment = Style::TEXT_ALIGN_BOTTOM;
    style->lineStroke = Style::LINE_DASH_DOTTED;
    plot->toggleGrid(true);
    plot->setGridColor(0x77777777);
    plot->setGridYStroke(Style::LINE_DOTTED);
    marks[0] = "0";
    marks[1] = "1";
    marks[2] = "2";
    marks[3] = "3";
    marks[4] = "4";
    marks[5] = "5";
    plot->draw(dataset, &marks, *style);
    currentOption = 1;
}

void ModelPlot::CreateOption2()
{

    if(currentOption == 2) return ;
    Clear();

    plot = new KbPlot(gl, -10.0, 50.0, -1.0, 50.0);

    points.push_back(Txy(0.0,0.0));
    points.push_back(Txy(1.0,1.0));
    points.push_back(Txy(-1.0,1.0));
    points.push_back(Txy(2.0,4.0));
    points.push_back(Txy(-2.0,4.0));
    points.push_back(Txy(3.0,9.0));
    points.push_back(Txy(-3.0,9.0));

    dataset = dynamic_cast<DataSet*>(new DSV(&points));

    style = new Style();
    style->lineColor = 0xAAFF67FF;
    style->lineThickness = 5.0;
    style->markerSize = 20;
    style->markerColor = 0xFF000055;
    style->markerType = Style::MARK_CROSS;
    style->lineStroke = Style::LineStroke::LINE_STD;
    plot->toggleGrid(true);
    plot->setGridColor(0x77777777);
    plot->setGridYStroke(Style::LINE_DASHED);
    plot->draw(dataset, *style);
    currentOption = 2;
}

void ModelPlot::CreateOption3()
{
    if(currentOption == 3) return ;
    Clear();

    plot = new KbPlot(gl, -10.0, 50.0, -1.0, 50.0);

    points.push_back(Txy(0.0,0.0));
    points.push_back(Txy(1.0,1.0));
    points.push_back(Txy(-1.0,1.0));
    points.push_back(Txy(2.0,4.0));
    points.push_back(Txy(-2.0,4.0));
    points.push_back(Txy(3.0,9.0));
    points.push_back(Txy(-3.0,9.0));

    dataset = dynamic_cast<DataSet*>(new DSV(&points));

    style = new Style();
    style->lineColor = 0x00FF00FF;
    style->lineThickness = 1.0;
    style->markerSize = 20;
    style->markerColor = 0xFF000055;
    style->markerType = Style::MARK_SQUARE;
    style->lineStroke = Style::LineStroke::LINE_DASHED;
    plot->toggleGrid(true);
    plot->setGridColor(0x777FFF77);
    plot->setGridYStroke(Style::LINE_DOTTED);
    plot->draw(dataset , *style);
    currentOption = 3;
}


void ModelPlot::CreateOption4()
{
    if(currentOption == 4) return ;
    Clear();

    plot = new KbPlot(gl, -10.0, 50.0, -1.0, 50.0);

    points.push_back(Txy(0.0,0.0));
    points.push_back(Txy(1.0,1.0));
    points.push_back(Txy(-1.0,1.0));
    points.push_back(Txy(2.0,4.0));
    points.push_back(Txy(-2.0,4.0));
    points.push_back(Txy(3.0,9.0));
    points.push_back(Txy(-3.0,9.0));

    dataset = dynamic_cast<DataSet*>(new DSV(&points));

    style = new Style();
    style->lineColor = 0x00FF00FF;
    style->lineThickness = 1.0;
    style->markerSize = 20;
    style->markerColor = 0xFF000055;
    style->markerType = Style::MARK_DOT;
    style->lineStroke = Style::LINE_DASH_DOTTED;
    plot->toggleGrid(true);
    plot->setGridColor(0x77777777);
    plot->setGridYStroke(Style::LINE_DOTTED);

    plot->draw(dataset, *style);
    currentOption = 4;

}

void ModelPlot::CreateOption5()
{
    if(currentOption == 5) return ;
    Clear();

    plot = new KbPlot(gl, -0.5, 1.0, -1.0, 10.0);


    for (int i = 0; i < 128; i+=2)
    {
        float x = 0.1 * (double)i;
        rawdata[i] = x;
        rawdata[i+1] = 5.0 * (1.0 - x + x*x/2.0 - x*x*x/6.0 + x*x*x*x/24.0 );
    }

    rd = new RawData<double>((unsigned char*)(&rawdata[0]),5, 16, 0, 8);
    dataset = dynamic_cast<DataSet*>(new DSRd(rd));
    style = new Style();
    style->lineColor = 0x00FF00FF;
    style->lineThickness = 1.0;
    style->fillEnable = true;
    style->fillColor = 0x0000FFFF;
    style->fillColorMax = 0xFFFFFFFF;
    style->extremumFill = true;
    plot->draw(dataset,*style);
    currentOption = 5;
}


