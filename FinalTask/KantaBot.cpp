#include <iostream>
#include <unistd.h>
#include "Grid.cpp"
#include "UtilityClasses.cpp"
using namespace std;
class KantaBot
{
private:
    Point loc;
    
public:
    Grid g;
    KantaBot(Grid g,Point loc);
    // KantaBot(int x,int y);
    void setBotLocation(int x,int y);
    // void setBotLocation(Point loc);
    Point getBotLocation();
    void sweepGrid(char d,int y1,int y2,int x1,int x2,int v);
    // void loadGrid();
    void moveVertical(int d);
    void moveHorizontal(int d);
    pair<Point,Point> sweepEndVertex(Cell);
    Cell* nextCell(Cell c);
    ~KantaBot();
};

KantaBot::KantaBot(Grid g,Point loc)
{
    this->g = g;
    this->loc = loc;
    this->g.grid[loc.y][loc.x] = 1;
}
// KantaBot::KantaBot(int x,int y)
// {
//     loc.x = x;
//     loc.y = y;
// }
void KantaBot::setBotLocation(int x,int y)
{
    loc = Point(x,y);
    g.grid[y][x]=1;
}
Point KantaBot::getBotLocation()
{
    return loc;
}
void KantaBot::sweepGrid(char d,int y1,int y2,int x1,int x2,int v) {
    int dir_row,dir_col;
    switch (v)
    {
    case 1:
        setBotLocation(x1,y1);//should check instead of set
        dir_row=1;
        dir_col=1;
        break;
    case 2:
        setBotLocation(x2,y1);
        dir_row = 1;
        dir_col = -1;
        break;
    case 3:
        setBotLocation(x1,y2);
        dir_row = -1;
        dir_col = 1;
        break;
    case 4:
        setBotLocation(x2,y2);
        dir_row = -1;
        dir_col = -1;
        break;
    default:
        cout<<"Wrong Vertex Option";
        break;
    }
    
    
    
    if(d=='H')
    {
        cout<<loc.y<<" "<<loc.x<<endl;
        for (; loc.y>=y1 && loc.y <=y2; loc.y+=dir_row)
        {
            for (; loc.x>=x1 && loc.x<=x2; loc.x+=dir_col)
            {
                g.grid[loc.y][loc.x]++;
                usleep(0.25*microsecond);
                g.display();
            }
            dir_col*=-1;
            loc.x+=dir_col;
        }
    }
    else if (d=='V')
    {
        for (; loc.x>=x1 && loc.x<=x2;loc.x+=dir_col)
        {
            for (; loc.y>=y1 && loc.y <=y2; loc.y+=dir_row)
            {
                g.grid[loc.y][loc.x]++;
                usleep(0.25*microsecond);
                g.display();
            }
            dir_row*=-1;
            loc.y+=dir_row;
        }
        
    }
    
}
void KantaBot::moveVertical(int d) {
    loc.y+=d;
    g.grid[loc.y][loc.x]++;
    usleep(0.25*microsecond);
}
void KantaBot::moveHorizontal(int d) {
    loc.x+=d;
    g.grid[loc.y][loc.x]++;
    usleep(0.25*microsecond);
}
KantaBot::~KantaBot()
{
}

pair<Point,Point> Kanta::sweepEndVertex(Cell c, int v)//Horizontal,Vertical,Also replace with bot location
{
    Point ph,pv;
    switch (v)
    {
    case 1:
        if (c.l%2==0)
        {
            ph.x = c.x1;
            ph.y = c.y2;
        }
        else
        {
            ph.x = c.x2;
            ph.y = c.y2;
        }
        
        if (c.b%2==0)
        {
            pv.x = c.x2;
            pv.y = c.y1;
        }
        else
        {
            pv.x = c.x2;
            pv.y = c.y2;
        }
        break;
    
    case 2:
        if (c.l%2==0)
        {
            ph.x = c.x2;
            ph.y = c.y2;
        }
        else
        {
            ph.x = c.x1;
            ph.y = c.y2;
        }
        
        if (c.b%2==0)
        {
            pv.x = c.x1;
            pv.y = c.y1;
        }
        else
        {
            pv.x = c.x1;
            pv.y = c.y2;
        }

        break;
    
    case 3:
        if (c.l%2==0)
        {
            ph.x = c.x1;
            ph.y = c.y1;
        }
        else
        {
            ph.x = c.x2;
            ph.y = c.y1;
        }
        
        if (c.b%2==0)
        {
            pv.x = c.x2;
            pv.y = c.y2;
        }
        else
        {
            pv.x = c.x2;
            pv.y = c.y1;
        }

        break;
    
    case 4:
        if (c.l%2==0)
        {
            ph.x = c.x2;
            ph.y = c.y1;
        }
        else
        {
            ph.x = c.x1;
            ph.y = c.y1;
        }
        
        if (c.b%2==0)
        {
            pv.x = c.x1;
            pv.y = c.y2;
        }
        else
        {
            pv.x = c.x1;
            pv.y = c.y1;
        }
        break;

    default:
        break;
    } 
    return make_pair(ph,pv);
}
Cell* KantaBot::nextCell(Cell c)//Replace parameter cell with bot location
{
    
}

int main()
{   Grid grid(10,10);
    grid.addObstacle(0,3,1,2);
    grid.addObstacle(3,6,4,8);
    KantaBot bot(grid,Point(0,0));
    for(int a:grid.getHorizontalSlices()){
        cout<<a;
    }
    // bot.g.display();
    cout<<endl;
    return 0;
}