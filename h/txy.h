
#ifndef TXY_H
#define TXY_H
struct Txy
{
	Txy(double _x, double _y){x = _x; y = _y;}
	Txy(){x = 0; y = 0;}
	double x,y;
    friend bool operator==(const Txy& left, const Txy& right);
};

bool operator==(const Txy& left, const Txy& right)
{
    return left.x == right.x && left.y == right.y;
}

#endif //TXY_H
