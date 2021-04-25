#include <iostream>
#include <unistd.h>
#include "stdc++.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "UtilityClasses.cpp"
using namespace std;
const unsigned int microsecond = 1000000;
class Grid
{
private:
    void mergeCells();
public:
    cv::Mat map;
    vector<vector<Cell> > layers;
    int grid[200][200]={};
    int l,b;
    Grid();
    Grid(int l,int b);
    ~Grid();
    void display();
    void addObstacle(int y1,int y2,int x1,int x2);
    vector<int> getHorizontalSlices();
    void createCells();
    bool isConnected(Cell a,Cell b);
    void convertToGrid();
    bool isGridSwept();
};
void Grid::convertToGrid()
{
    l=b=25;
    cv::Mat temp;
    resize(map,temp,cv::Size_<int>(l,b));//just to see the output
    cv::bitwise_not(temp,temp);
    // cv::imshow("Image",temp);
    // cv::waitKey();
    for(int i = 0 ;i < l; ++i)
        for(int j = 0; j < b; ++j)
        {
            if(temp.at<uchar>(i,j)<100)
                grid[i][j]=0;
            else
                grid[i][j]=temp.at<uchar>(i,j);
        }
    
}
bool Grid::isGridSwept()
{
    for(vector<vector<Cell> >::iterator l = layers.begin(); l<layers.end(); l++)
    {
        for(vector<Cell>::iterator c = l->begin();c<l->end();c++)
        {
                if(c->swept==false)
                {
                    return false;
                }
        }
    }
    return true;
}
Grid::Grid() {
    map = cv::imread("imgs/test_img5.jpeg",0);
    convertToGrid();
}
Grid::Grid(int l,int b)
{
    this->l= l;
    this->b= b;
}

Grid::~Grid()
{
}

void Grid::display() {
    system("clear");
    cout<<"=============================================================="<<endl;
    cout<<"=============================================================="<<endl;
    cout<<"=============================================================="<<endl;
    for (int i = 0; i < l; i++)
    {
        for (int j = 0; j < b; j++)
            cout<<grid[i][j]<<" \t";
        cout<<endl<<endl;
    }
    
    // usleep(0.25*microsecond);
}

void Grid::addObstacle(int y1,int y2,int x1,int x2)
{
    system("clear");
    for (int i = y1; i <=y2; i++)
    {
        for (int j = x1; j <=x2; j++)
        {
            grid[i][j]+=100;
        }
        
    }
    
}

vector<int> Grid::getHorizontalSlices()
{
    vector<int> s;
    s.push_back(0);
    for(int r = 1; r<l; r++)
    {
        for(int c = 0; c<b; c++)
        {
            if(grid[r-1][c]!=grid[r][c])
            {
                s.push_back(r);
                break;
            }
        }
    }
    s.push_back(l);
    return s;
}

void Grid::createCells() {
    vector<int> s = getHorizontalSlices();
    int n = s.size();
    bool cellOpen = false;
    Cell c;
    for(int i = 0; i < n-1; i++)    {
        cellOpen = false;
        vector<Cell> layer;
        for(int col=0; col<b; col++)
        {
            if (grid[s[i]][col]==0 && !cellOpen)
            {
                c = Cell(col,s[i],0,s[i+1]-1,i);
                cellOpen = true;
            }
            else if(grid[s[i]][col]>=100 && cellOpen)
            {
                c.x2 = col-1;
                c.b = c.x2 - c.x1 + 1;
                layer.push_back(c);
                cellOpen = false;
            }
        }
        if(cellOpen)
        {
            c.x2 = b-1;
            c.b = c.x2 -c.x1 + 1;
            layer.push_back(c);
            cellOpen = false;
        }
        layers.push_back(layer);
    }
    // mergeCells();
}

void Grid::mergeCells()
{
    for(int i=0;i<layers.size()-1;i++)
    {
        for(vector<Cell>::iterator j =layers[i].begin(),k=layers[i+1].begin(); j<layers[i].end() && k<layers[i+1].end(); )
        {
            if(j->x1 == k->x1 && j->x2 == k->x2)
            {
                k->y1 = j->y1;
                k->l = k->y2 - k->y1;
                j = layers[i].erase(j);        
            }
            else if(j->x1 > k->x1)
                k++;
            else
                j++;
        }
    }
}

bool Grid::isConnected(Cell a,Cell b)
{
    if(a.y2+1 == b.y1 || b.y2+1 == a.y1)
    {
        if((b.x1>=a.x1 && b.x1<=a.x2) || (b.x2>=a.x1 && b.x2<=a.x2))
            return true;
        else if((a.x1>=b.x1 && a.x1<=b.x2) || (a.x2>=b.x1 && a.x2<=b.x2))
            return true;
        else 
            return false;
    }
    return false;
}