#ifndef TXY_H
#define TXY_H
struct Txy 
{
	Txy(double _x, double _y){x = _x; y = _y;}
	Txy(){x = 0; y = 0;}
	double x,y;
};
#endif
