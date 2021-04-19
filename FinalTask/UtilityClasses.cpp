class Point
{
public:
    int x=0,y=0;
    Point();
    Point(int x,int y);
    ~Point();
};
Point::Point() {

}
Point::Point(int x,int y)
{
    this->x = x;
    this->y = y;
}

Point::~Point()
{
}
class Cell
{
private:
public:
    int y1,y2,x1,x2;
    int l,b;
    bool swept;
    Cell* nextCell;
    Cell();
    Cell(int y1,int y2,int x1,int x2);
    Point* getVertices();
    ~Cell();
};
Cell::Cell()
{
    x1=x2=y1=y2=l=b=0;
    swept = false;
}
Cell::Cell(int x1, int y1, int x2,int y2)
{
    this->y1 = y1;
    this->y2 = y2;
    this->x1 = x1;
    this->x2 = x2;
    l = y2 - y1 + 1;
    b = x2 - x1 + 1;
    swept = false;
}

Point* Cell::getVertices()
{
    Point v[] = {Point(x1,y1),Point(x2,y1),Point(x1,y2),Point(x2,y2)};
    return v;
}

Cell::~Cell()
{
}
