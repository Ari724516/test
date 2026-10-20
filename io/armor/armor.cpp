#include "armor.h"

ArmorDetector::ArmorDetector()
{
    light_min_ = 200;
    light_max_ = 255;
    min_area_ = 20;
    max_area_ = 2000;
}

ArmorDetector::~ArmorDetector() {}

std::vector<std::vector<cv::Point>> ArmorDetector::findLightBars(const cv::Mat &gray)
{
    cv::Mat bin;
    cv::threshold(gray, bin, light_min_, light_max_, cv::THRESH_BINARY);
    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(bin, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

    std::vector<std::vector<cv::Point>> lights;
    for(auto &c : contours)
    {
        double area = cv::contourArea(c);
        if(area > min_area_ && area < max_area_)
        {
            lights.push_back(c);
        }
    }
    return lights;
}
s