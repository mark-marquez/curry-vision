#include <iostream>
#include <curryvision/video_source.hpp>

int main() {
    std::cout << "Hi, Curry Vision here!" << std::endl;

    VideoSource video; 

    if (!video.isOpen()) {
        return -1; // camera failed to open
    }

    while (true) {
        cv::Mat frame = video.grabFrame();
        if (frame.empty()) break;
        cv::imshow("Test", frame);
        if (cv::waitKey(1) == 27) break;
    }
    

    return 0;
}
