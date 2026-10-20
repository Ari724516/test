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

bool ArmorDetector::isArmorPair(const std::vector<cv::Point> &l1, const std::vector<cv::Point> &l2)
{
    cv::RotatedRect r1 = cv::minAreaRect(l1);
    cv::RotatedRect r2 = cv::minAreaRect(l2);
    float dy = fabs(r1.center.y - r2.center.y);
    float dx = fabs(r1.center.x - r2.center.x);
    if(dy < 40 && dx > 30)
        return true;
    return false;
}

int ArmorDetector::recognizeNumber(const cv::Mat &numRoi)
{
    cv::Mat gray;
    cv::cvtColor(numRoi, gray, cv::COLOR_BGR2GRAY);
    cv::threshold(gray, gray, 100,255,cv::THRESH_BINARY);
    int white = cv::countNonZero(gray);
    if(white > 200) return 1;
    else return 0;
}

cv::Mat ArmorDetector::detect(cv::Mat src)
{
    cv::Mat gray;
    cv::cvtColor(src, gray, cv::COLOR_BGR2GRAY);
    auto lights = findLightBars(gray);

    for(int i = 0; i < lights.size(); i++)
    {
        for(int j = i+1; j < lights.size(); j++)
        {
            if(isArmorPair(lights[i], lights[j]))
            {
                cv::RotatedRect r1 = cv::minAreaRect(lights[i]);
                cv::RotatedRect r2 = cv::minAreaRect(lights[j]);
                cv::Point2f pts1[4], pts2[4];
                r1.points(pts1);
                r2.points(pts2);
                std::vector<cv::Point2f> all;
                for(int k=0;k<4;k++) {all.push_back(pts1[k]); all.push_back(pts2[k]);}
                cv::RotatedRect armor_rect = cv::minAreaRect(all);

                cv::Point2f armorPts[4];
                armor_rect.points(armorPts);
                for(int k=0;k<4;k++)
                {
                    cv::line(src, armorPts[k], armorPts[(k+1)%4], cv::Scalar(0,255,0),2);
                }
                cv::Point2f center = armor_rect.center;
                std::cout << "Armor center x:" << center.x << " y:" << center.y << std::endl;

                cv::Rect roi(armor_rect.center.x -20, armor_rect.center.y -15,40,30);
                if(roi.x>0 && roi.y>0 && roi.br().x < src.cols && roi.br().y < src.rows)
                {
                    cv::Mat numImg = src(roi);
                    int num = recognizeNumber(numImg);
                    cv::putText(src, std::to_string(num), roi.tl(), cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(0,0,255),2);
                }
            }
        }
    }
    return src;
}
