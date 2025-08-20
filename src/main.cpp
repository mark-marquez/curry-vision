#include <iostream>
#include <opencv2/opencv.hpp>

int main() {
    std::cout << "Hi, Curry Vision here!" << std::endl;

    std::cout << "Using OpenCV version: " << CV_VERSION << std::endl;

    // Optional: create a blank image and show it
    cv::Mat img = cv::Mat::zeros(200, 400, CV_8UC3);
    cv::putText(img, "Hello OpenCV", {30, 100},
                cv::FONT_HERSHEY_SIMPLEX, 1, {255, 255, 255}, 2);

    cv::imshow("Test Window", img);
    cv::waitKey(0);  // wait for a key press

    return 0;
}
