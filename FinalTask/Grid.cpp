#include <iostream>
#include <unistd.h>
#include "stdc++.h"
#include "UtilityClasses.cpp"
using namespace std;
const unsigned int microsecond = 1000000;
class Grid
{
private:
    void mergeCells(vector<Cell>*,int);
public:
    int grid[20][20]={};
    int l,b;
    Grid();
    Grid(int l,int b);
    ~Grid();
    void display();
    void addObstacle(int y1,int y2,int x1,int x2);
    vector<int> getHorizontalSlices();
    vector<Cell>* createCells();
};
Grid::Grid() {
    l=0;
    b=0;
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
    for (int i = 0; i < l; i++)
    {
        for (int j = 0; j < b; j++)
            cout<<grid[i][j]<<"\t";
        cout<<endl<<endl;
    }
    cout<<"=============================================================="<<endl;
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

vector<Cell>* Grid::createCells() {
    vector<int> s = getHorizontalSlices();
    int n = s.size()-1;
    vector<Cell> layers[n];
    bool cellOpen = false;
    Cell c;
    for(int i = 0; i < n-1; i++)    {
        cellOpen = false;
        for(int col=0; col<b; col++)
        {
            if (grid[s[i]][col]==0 && !cellOpen)
            {
                c = Cell(col,s[i],0,s[i+1]-1);
                cellOpen = true;
            }
            else if((grid[s[i]][col]>=100 || col == b-1) && cellOpen)
            {
                c.x2 = col-1;
                layers[i].push_back(c);
                cellOpen = false;
            }
        }
    }
    mergeCells(layers,n);
    return layers;
}

void Grid::mergeCells(vector<Cell>* layers,int n)
{
    for(int i=0;i<n-1;i++)
    {
        for(vector<Cell>::iterator j =layers[i].begin(),k=layers[i+1].begin(); j<layers[i].end() && k<layers[i+1].end(); )
        {
            if(j->x1 == k->x1 && j->x2 == k->x2)
            {
                k->y1 = j->y1;
                j = layers[i].erase(j);        
            }
            else if(j->x1 > k->x1)
                k++;
            else
                j++;
        }
    }
}