#include <iostream>
#include <opencv2/opencv.hpp>
using namespace cv;
using namespace std;
int main()
{
    Mat img = imread("/home/ari/Desktop/green_paper.png");
    cout << "rows: " << img.rows << " cols: " << img.cols << endl;
    return 0;
}

