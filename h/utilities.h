#ifndef UTILITIES_H
#define UTILITIES_H

#include "utilities.h"
#include "txy.h"
#include <GL/gl.h>
#include <GL/glu.h>

/* Набор утилит, позволяющих не зная границ рабочей области
 * конвертировать различные координаты. Убирают необходимость подавать
 * параметрами графическим объектам координаты рабочей области */

Txy myglTrObjectScreen(double x, double y); //Математику в координаты экрана
Txy myglTrScreenObject(double x, double y); //К-ы экрана в математику
Txy myglPlaneWH();//Получить размеры видимой плоскости (ширина, высота)
Txy myglScreenWH();//Получить размеры экрана(ш,в)

#endif
