#include <opencv2/opencv.hpp>
#include "armor/armor.h"

int main()
{
    cv::VideoCapture cap(2);
    if(!cap.isOpened())
    {
        std::cout << "摄像头打开失败" << std::endl;
        return -1;
    }
    ArmorDetector detector;
    cv::Mat frame;
    while(cap.read(frame))
    {
        cv::Mat out = detector.detect(frame);
        cv::imshow("Armor Detect", out);
        int key = cv::waitKey(1);
        if(key == 27) break;
    }
    cap.release();
    cv::destroyAllWindows();
    return 0;
}
