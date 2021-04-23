using namespace std;
#include <array>
class Point
{
public:
    int x,y;
    Point();
    Point(int x,int y);
    float pointDistance(Point a);
    ~Point();
};
Point::Point() {
    x=y=0;
}
Point::Point(int x,int y)
{
    this->x = x;
    this->y = y;
}

float Point::pointDistance(Point a)
{
    return pow(a.x-this->x,2) + pow(a.y - this->y,2);
}

Point::~Point()
{
}



class Cell
{
private:
public:
    int layer;
    int y1,y2,x1,x2;
    int l,b;
    bool swept;
    Cell();
    Cell(int x1,int y1,int x2,int y2,int layer);
    friend bool operator== (Cell & lhs, Cell & rhs );
    array<Point,4> getVertices()
    {
        array<Point,4> v;
        v[0] = Point(x1,y1);
        v[1] = Point(x2,y1);
        v[2] = Point(x1,y2);
        v[3] = Point(x2,y2);
        return v;
    }
    ~Cell();
};
Cell::Cell()
{
    x1=x2=y1=y2=l=b=layer=0;
    swept = false;
}
Cell::Cell(int x1, int y1, int x2,int y2,int layer)
{
    this->y1 = y1;
    this->y2 = y2;
    this->x1 = x1;
    this->x2 = x2;
    this->layer = layer;
    l = y2 - y1 + 1;
    b = x2 - x1 + 1;
    swept = false;
}
Cell::~Cell()
{
}
bool operator== ( Cell & lhs, Cell & rhs )
{
    if (lhs.x1 == rhs.x1 && lhs.x2 == rhs.x2 && lhs.y1 == rhs.y1 && lhs.y2 == rhs.y2)
    {
        return true;
    }
    else
        return false;
    
}