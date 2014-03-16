//---------------------------------------------------------------------------

#ifndef KBPLOT_H
#define KBPLOT_H

#include "glwidget.h"
#include "event.h"
#include "txy.h"

using std::vector;

class KbPlot : IMouseEventListener
{
    /*
     * Главная сущность библиотеки.
     */
public:

    KbPlot(GLWidget*, double xmin, double xmax, double ymin, double ymax);
    KbPlot(GLWidget*) : mouseNewMovement(true) { }
    ~KbPlot();

    void setRanges(double xmin, double xmax, double ymin, double ymax);

    void setBackground(const string filename);
    void setBackground(const unsigned int color);

    // Настройка шага отметок по двум осям
    void setAxisTick(double);
    // Настройка шага отметок по х или у
    void setAxisXTick(double);
    void setAxisYTick(double);

    // Задание имени осей абсцисс и ординат. НЕ РЕАЛИЗОВАНО
    void setAxisXName(const string);
    void setAxisYName(const string);

    //Включить сетку
    void toggleGrid(bool b);

    //Установка шага сетки
    void setGridTick(const int);
    void setGridXTick(const int);
    void setGridYTick(const int);

    //Установка штриха сетки
    void setGridStroke(const Style::LineStroke)  ;
    void setGridXStroke(const Style::LineStroke) ;
    void setGridYStroke(const Style::LineStroke) ;

    //Установка толщины сетки
    void setGridThickness(const unsigned int);
    //Установка цвета сетки
    void setGridColor(const unsigned int);

    //Построение набора данных
    //Стиль: стандартный
    //Текстовые метки: нет
    void draw(DataSet *ds);

    //Построение набора данных
    //Стиль: пользовательский
    //Текстовые метки: нет
    void draw(DataSet *ds, Style &s);

    //Построение набора данных
    //Стиль: стандартный
    //Текстовые метки: есть
    void draw(DataSet *ds,std::map<size_t, std::string>  *tmarks);

    //Построение набора данных
    //Стиль: пользовательский
    //Текстовые метки: есть
    void draw(DataSet *ds,std::map<size_t, std::string>  *tmarks, Style &s);

    //Сохранить bmp-изображение под указанным именем
    void exportAsImage(const string filename);

    virtual void mouseMoveEvent(int x,int y);
    virtual void mousePressedEvent(int x,int y);
    virtual void mouseReleaseEvent(int x,int y);
    virtual void mouseScrollEvent(int angle);
private:

    //Константы с настройками отображения
    static const double c_frameThickness;
    static const double c_tickLength;
    static const double c_fontSize;

    //Вспомогательная функция, подстраивающая начало степени на подходящую степень 10
    double axisAdjustMin(double min, double tick);

    //Перестроение отметок на осях и сетки.
    //Используются при изменении границ рабочей области.
    void axisRebuild();
    void gridRebuild();

    std::vector<Line*> xticks_t;
    std::vector<Line*> xticks_b;
    std::vector<Line*> yticks_r;
    std::vector<Line*> yticks_l;

    std::vector<Text*> xmrk_t, xmrk_b;
    std::vector<Text*> ymrk_l, ymrk_r;

    std::vector<Line*> xgrid;
    std::vector<Line*> ygrid;

    std::vector<Txy> *framePoints;
    DSV *framePointsDS;

    GLWidget *container;
    bool mouseNewMovement;
    bool mouseLButtonDown;  		// событие перемещения сработает только с зажатой ЛКМ
    double xmax, ymax, xmin, ymin;
    double xtick, ytick;
    double xgtick, ygtick;
    bool  isGridEnabled;

    Style stAxisX, stAxisY, stBkgnd, stGridX, stGridY;
};

#endif //KBPLOT_H
