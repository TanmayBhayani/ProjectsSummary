#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
using namespace std;
using namespace cv;
int main()
{
    Mat map = imread("test_img3.jpeg");
    Mat temp;
    resize(map,temp,Size_<int>(125,125));
    cout<<map.size();
    imshow("Image",temp);
    waitKey();
    return 0;
}