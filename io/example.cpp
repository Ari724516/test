#include <iostream>
#include <opencv2/opencv.hpp>

class MyCamera
{
private:
    cv::VideoCapture cap;
public:
    MyCamera(int idx)
    {
        cap.open(idx, cv::CAP_V4L2);
        if (!cap.isOpened())
        {
            std::cout << "Camera failed to open.camera id = " << idx << std::endl;
        }
    }

   
    bool readFrame(cv::Mat &frame)
    {
        return cap.read(frame);
    }

   
    bool isOpen()
    {
        return cap.isOpened();
    }

    
    ~MyCamera()
    {
        cap.release();
        cv::destroyAllWindows();
        std::cout << "camera was released" << std::endl;
    }
};

int main()
{
    MyCamera cam(2);
    if (!cam.isOpen())
    {
        return -1;
    }

    cv::Mat frame;
    std::cout << "Reading screen,press ESC to exit" << std::endl;
    while (true)
    {
        if (!cam.readFrame(frame))
        {
            std::cout << "Failed to read image" << std::endl;
            break;
        }
        cv::imshow("camera", frame);
        int key = cv::waitKey(1);
        if (key == 27)
        {
            break;
        }
    }
    return 0;
}
