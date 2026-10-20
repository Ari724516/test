#ifndef ARMOR_H
#define ARMOR_H
#include <opencv2/opencv.hpp>

class ArmorDetector {
public:
    ArmorDetector();
    ~ArmorDetector();
    cv::Mat detect(cv::Mat src);

private:
    int light_min_;
    int light_max_;
    double min_area_;
    double max_area_;

    std::vector<std::vector<cv::Point>> findLightBars(const cv::Mat &gray);
    bool isArmorPair(const std::vector<cv::Point> &l1, const std::vector<cv::Point> &l2);
    int recognizeNumber(const cv::Mat &numRoi);
};

#endif