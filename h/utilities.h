п»ї#ifndef UTILITIES_H
#define UTILITIES_H

#include "txy.h"
#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>

/* РќР°Р±РѕСЂ СѓС‚РёР»РёС‚, РїРѕР·РІРѕР»СЏСЋС‰РёС… РЅРµ Р·РЅР°СЏ РіСЂР°РЅРёС† СЂР°Р±РѕС‡РµР№ РѕР±Р»Р°СЃС‚Рё
 * РєРѕРЅРІРµСЂС‚РёСЂРѕРІР°С‚СЊ СЂР°Р·Р»РёС‡РЅС‹Рµ РєРѕРѕСЂРґРёРЅР°С‚С‹. РЈР±РёСЂР°СЋС‚ РЅРµРѕР±С…РѕРґРёРјРѕСЃС‚СЊ РїРѕРґР°РІР°С‚СЊ
 * РїР°СЂР°РјРµС‚СЂР°РјРё РіСЂР°С„РёС‡РµСЃРєРёРј РѕР±СЉРµРєС‚Р°Рј РєРѕРѕСЂРґРёРЅР°С‚С‹ СЂР°Р±РѕС‡РµР№ РѕР±Р»Р°СЃС‚Рё */

Txy myglTrObjectScreen(double x, double y); //РњР°С‚РµРјР°С‚РёРєСѓ РІ РєРѕРѕСЂРґРёРЅР°С‚С‹ СЌРєСЂР°РЅР°
Txy myglTrScreenObject(double x, double y); //Рљ-С‹ СЌРєСЂР°РЅР° РІ РјР°С‚РµРјР°С‚РёРєСѓ
Txy myglPlaneWH();//РџРѕР»СѓС‡РёС‚СЊ СЂР°Р·РјРµСЂС‹ РІРёРґРёРјРѕР№ РїР»РѕСЃРєРѕСЃС‚Рё (С€РёСЂРёРЅР°, РІС‹СЃРѕС‚Р°)
Txy myglScreenWH();//РџРѕР»СѓС‡РёС‚СЊ СЂР°Р·РјРµСЂС‹ СЌРєСЂР°РЅР°(С€,РІ)


#endif //UTILITIES_H
