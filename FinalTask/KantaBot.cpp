#include <iostream>
#include <unistd.h>
#include "Grid.cpp"

using namespace std;
class KantaBot
{
private:
    Point loc;
    
public:
    Grid g;
    stack<Cell> back_track;
    KantaBot(Grid g,Point loc);
    // KantaBot(int x,int y);
    void setBotLocation(int x,int y);
    // void setBotLocation(Point loc);
    Point getBotLocation();
    void sweepGrid(char d);
    // void loadGrid();
    pair<char,Point> getSweepDirection(pair<Point,Point>,array<Point,4>);
    Cell& getCurrentCell();
    void run();
    void moveVertical(int d);
    void moveHorizontal(int d);
    pair<Point,Point> sweepEndVertex();
    Cell* nextCell();
    void moveToCell(Cell c,Point v);
    void markCellAsSwept(Cell c);
    Point getClosestVertex(Cell c);
    ~KantaBot();
};

KantaBot::KantaBot(Grid g,Point loc)
{
    this->g = g;
    this->loc = loc;
    // this->g.grid[loc.y][loc.x] = 1;
}
void KantaBot::setBotLocation(int x,int y)
{
    loc = Point(x,y);
    g.grid[y][x]=1;
}
Point KantaBot::getBotLocation()
{
    return loc;
}
void KantaBot::sweepGrid(char d) {
    int dir_row,dir_col;
    Cell c = getCurrentCell();
    int v;
    if(loc.x==c.x1 && loc.y == c.y1)
    {
        v=0;
        dir_row=1;
        dir_col=1;
    }
    else if (loc.x==c.x2 && loc.y == c.y1)
    {
        v=1;
        dir_row = 1;
        dir_col = -1;
    }
    else if (loc.x == c.x1 && loc.y == c.y2)
    {
        v=2;
        dir_row = -1;
        dir_col = 1;
    }
    else if (loc.x == c.x2 && loc.y == c.y2)
    {
        v=3;
        dir_row = -1;
        dir_col = -1;
    }
    else
        cout<<"BOT NOT at Vertex";
    
    if(d=='H')
    {
        // cout<<loc.y<<" "<<loc.x<<endl;
        for (; (v>=2 && loc.y>c.y1) || (v<=1 && loc.y <c.y2); moveVertical(dir_row))
        {
            moveHorizontal(dir_col*(c.b-1));
            dir_col*=-1;
        }
        moveHorizontal(dir_col*(c.b-1));
    }
    else if (d=='V')
    {
        for (; (v%2==1 && loc.x>c.x1) || (v%2==0 && loc.x<c.x2);moveHorizontal(dir_col))
        {
            moveVertical(dir_row*(c.l-1));
            dir_row*=-1;
        }
        moveVertical(dir_row*(c.l-1));
    }
    markCellAsSwept(c);
}
void KantaBot::moveVertical(int d) {
    int dir = (d>0)? 1:-1;
    for(int i=1;i<=abs(d);i++)
    {
        loc.y+=dir;
        g.grid[loc.y][loc.x]++;
        cv::rectangle(g.map,cv::Point_<int>(loc.x*g.scale,loc.y*g.scale),cv::Point_<int>((loc.x*g.scale)+g.scale-1,(loc.y*g.scale)+g.scale-1),cv::Scalar_<uchar>(60),cv::FILLED);
        cv::Mat temp;
        g.map.copyTo(temp);
        cv::circle(temp,cv::Point_<int>(loc.x*g.scale+g.scale/2,loc.y*g.scale+g.scale/2),g.scale/2,cv::Scalar_<uchar>(20),cv::FILLED);
        // g.display();
        cv::imshow("Map",temp);
        cv::waitKey(1);
        usleep(0.01*microsecond);
    }
}
void KantaBot::moveHorizontal(int d) {
    int dir = (d>0)? 1:-1;
    for(int i=1;i<=abs(d);i++)
    {
        loc.x+=dir;
        g.grid[loc.y][loc.x]++;
        cv::rectangle(g.map,cv::Point_<int>(loc.x*g.scale,loc.y*g.scale),cv::Point_<int>((loc.x*g.scale)+g.scale-1,(loc.y*g.scale)+g.scale-1),cv::Scalar_<uchar>(60),cv::FILLED);
        cv::Mat temp;
        g.map.copyTo(temp);
        cv::circle(temp,cv::Point_<int>(loc.x*g.scale+g.scale/2,loc.y*g.scale+g.scale/2),g.scale/2,cv::Scalar_<uchar>(20),cv::FILLED);
        // g.display();
        cv::imshow("Map",temp);
        cv::waitKey(1);
        usleep(0.01*microsecond);
    }
}
KantaBot::~KantaBot()
{
}

pair<Point,Point> KantaBot::sweepEndVertex()//Horizontal,Vertical,Also replace with bot location
{
    Point ph,pv;
    Cell c = getCurrentCell();
    if(loc.x==c.x1 && loc.y == c.y1)
    {
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
    }
    else if (loc.x==c.x2 && loc.y == c.y1)
    {
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
    }
    else if (loc.x == c.x1 && loc.y == c.y2)
    {
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
    }
    else if (loc.x == c.x2 && loc.y == c.y2)
    {
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
    }
    else
        cout<<"BOT NOT at Vertex";
    return make_pair(ph,pv);
}

Cell& KantaBot::getCurrentCell()//returns Copy!!!!!
{
    for(vector<vector<Cell> >::iterator layer = g.layers.begin(); layer<g.layers.end();layer++)
    {
        for(vector<Cell>::iterator cell = layer->begin();cell<layer->end();cell++)
        {
            if(loc.x>=cell->x1 && loc.x<=cell->x2 && loc.y>=cell->y1 && loc.y<=cell->y2)
                return *cell;
        }
    }
}

// Cell* KantaBot::nextCell()//Replace parameter cell with bot location
// {
//     Cell curr_cell = getCurrentCell();
//     for(vector<vector<Cell> >::iterator l = g.layers.begin(); l<g.layers.end(); l++)
//     {
//         for(vector<Cell>::iterator c = l->begin();c<l->end();c++)
//         {
//                 if(g.isConnected(curr_cell,*c) && c->swept==false && !(*c==curr_cell))
//                 {
//                     return &*c;
//                 }
//         }
//     }
//     return nullptr;
// }

Cell* KantaBot::nextCell()
{
    Cell curr_cell = getCurrentCell();
    if(curr_cell.layer!=0)
    {
        for (vector<Cell>::iterator c = g.layers[curr_cell.layer-1].begin(); c < g.layers[curr_cell.layer-1].end(); c++)
        {
            if(g.isConnected(curr_cell,*c) && c->swept==false && !(*c==curr_cell))
            {
                return &*c;
            }
        }
    }
    if(curr_cell.layer!=g.layers.size()-1)
    {
        for (vector<Cell>::iterator c = g.layers[curr_cell.layer+1].begin(); c < g.layers[curr_cell.layer+1].end(); c++)
        {
            if(g.isConnected(curr_cell,*c) && c->swept==false && !(*c==curr_cell))
            {
                return &*c;
            }
        }
    }
    
    return nullptr;
}

pair<char,Point> KantaBot::getSweepDirection(pair<Point,Point> p,array<Point,4> v)
{
    float hd=9999999,vd=9999999;
    Point closest_vertexH,closest_vertexV;
    for(int i=0; i<4; i++)
    {
        if(v[i].pointDistance(p.first)<hd)
        {
            hd = v[i].pointDistance(p.first);
            // cout<<hd<<"-"<<"("<<v[i].x<<","<<v[i].y<<")"<<","<<"("<<p.first.x<<","<<p.first.y<<")"<<endl;
            closest_vertexH = v[i];
        }
        if(v[i].pointDistance(p.second)<vd)
        {
            vd = v[i].pointDistance(p.second);
            closest_vertexV = v[i];
        }
    }
    if(hd<=vd)
        return make_pair('H',closest_vertexH);
    else
        return make_pair('V',closest_vertexV);
}

void KantaBot::moveToCell(Cell c,Point v)
{
    Cell curr_cell = getCurrentCell();
    int dir;
    if(g.isConnected(c,curr_cell))
    {
        if(loc.x >= c.x1 && loc.x <= c.x2)
        {
            moveVertical(v.y-loc.y);
            moveHorizontal(v.x-loc.x);
        }
        else
        {
            moveHorizontal(v.x-loc.x);
            moveVertical(v.y-loc.y);
        }
    }
}
Point KantaBot::getClosestVertex(Cell c)
{
    float d=99999;
    Point t;
    for (Point p:c.getVertices())
    {
        if(loc.pointDistance(p)<d)
        {
            t = p;
            d = loc.pointDistance(p);
        }
    }
    return t;
}
void KantaBot::markCellAsSwept(Cell c)
{
    for(vector<Cell>::iterator cell = g.layers[c.layer].begin();cell<g.layers[c.layer].end();cell++)
    {
        if(*cell==c)
        {
            cell->swept=true;
            return;
        }
    }
    cout<<"ERROR";
}
void KantaBot::run()
{   g.createCells();
    setBotLocation(0,0);

    // Cell c = getCurrentCell();
    // cout<<c.b;
    Cell *nxt_cell;
    while (!g.isGridSwept())
    {
        if(nxt_cell = nextCell())
        {
            if(getCurrentCell().swept==false)
            {
                pair<char,Point> n = getSweepDirection(sweepEndVertex(),nxt_cell->getVertices());
                sweepGrid(n.first);
                back_track.push(getCurrentCell());
                moveToCell(*nxt_cell,n.second);
            }
            else
            {
                moveToCell(*nxt_cell,getClosestVertex(*nxt_cell));
            }
        }
        else if(getCurrentCell().swept==true)
        {
            back_track.pop();
            moveToCell(back_track.top(),getClosestVertex(back_track.top()));
        }
        else
        {
            pair<char,Point> n = getSweepDirection(sweepEndVertex(),back_track.top().getVertices());
            sweepGrid(n.first);
            moveToCell(back_track.top(),getClosestVertex(back_track.top()));
        }
        
    }
    sweepGrid('H');
    cout<<endl;
}
int main()
{   
    // Grid grid(10,10);
    // grid.addObstacle(5,10,0,3);
    // grid.addObstacle(5,10,6,9);
    //-----------------------
    // grid.grid[2][4]=100;
    // grid.addObstacle(3,3,3,5);
    // grid.addObstacle(4,4,2,6);
    // grid.addObstacle(5,5,1,7);
    Grid g;
    KantaBot bot(g,Point(0,0));
    bot.run();
    // bot.g.display();
    return 0;
}