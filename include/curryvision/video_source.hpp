#pragma once

#include <opencv2/opencv.hpp>

class VideoSource {
    public:
        explicit VideoSource(int camera_index = 0);

        // Adding to be explicit, could be skipped
        ~VideoSource();

        bool isOpen() const;
        cv::Mat grabFrame();

    private:
        cv::VideoCapture video_;
        
};