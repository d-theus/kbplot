#ifndef UTILITIES_H
#define UTILITIES_H

/*
 * Вспомогательные функции для трансляции координат и получении геометрий.
 * Используется только openGL, то есть не нужны объекты GLWidget или KbPlot
 */
#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>

// Транслировать координаты на плоскости в координаты окна
Txy myglTrObjectScreen(double x, double y);

// Транслировать координаты на окне в координаты на плоскости
Txy myglTrScreenObject(double x, double y);

// Получить ширину и высоту отображаемого участка плоскости
Txy myglPlaneWH();

// Получить
Txy myglScreenWH();


#endif //UTILITIES_H

