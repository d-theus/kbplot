
#ifndef TXY_H
#define TXY_H
struct Txy
{
    Txy(double _x = 0.0, double _y = 0.0)
    {
        x = _x;
        y = _y;
    }

    double x,y;

    friend bool operator==(const Txy& left, const Txy& right);
};

bool operator==(const Txy& left, const Txy& right)
{
    return left.x == right.x && left.y == right.y;
}

#endif //TXY_H
