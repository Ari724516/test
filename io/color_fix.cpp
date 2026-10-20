#include <iostream>
#include <opencv2/opencv.hpp>

cv::Mat colorCorrection(const cv::Mat &src)
{
    std::vector<cv::Mat> channels;
    cv::split(src, channels);
    cv::Mat B = channels[0];
    cv::Mat G = channels[1];
    cv::Mat R = channels[2];

    cv::Scalar meanB = cv::mean(B);
    cv::Scalar meanG = cv::mean(G);
    cv::Scalar meanR = cv::mean(R);

    double mean_B = meanB[0];
    double mean_G = meanG[0];
    double mean_R = meanR[0];

    double mean_all = (mean_B + mean_G + mean_R) / 3.0;

    double gainB = mean_all / mean_B;
    double gainG = mean_all / mean_G;
    double gainR = mean_all / mean_R;

    cv::Mat dstB, dstG, dstR;
    B.convertTo(dstB, CV_64F);
    G.convertTo(dstG, CV_64F);
    R.convertTo(dstR, CV_64F);

    dstB = dstB * gainB;
    dstG = dstG * gainG;
    dstR = dstR * gainR;

    dstB.convertTo(dstB, CV_8U);
    dstG.convertTo(dstG, CV_8U);
    dstR.convertTo(dstR, CV_8U);

    std::vector<cv::Mat> outChannels = {dstB, dstG, dstR};
    cv::Mat result;
    cv::merge(outChannels, result);
    return result;
}

int main()
{
    cv::Mat img = cv::imread("/home/ari/Desktop/green_paper.png");
    if (img.empty())
    {
        std::cout << "图片读取失败！检查图片路径和文件名" << std::endl;
        return -1;
    }

    cv::Mat corrected = colorCorrection(img);

    cv::imshow("原始带绿色偏色图片", img);
    cv::imshow("色彩校正之后图片", corrected);

    cv::imwrite("./corrected_result.jpg", corrected);

    cv::waitKey(0);
    cv::destroyAllWindows();
    return 0;
}
