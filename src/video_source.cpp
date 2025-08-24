#include "curryvision/video_source.hpp"

VideoSource::VideoSource(int camera_index) : video_(camera_index) {}

VideoSource::~VideoSource() {
    if (video_.isOpened()) {
        video_.release();
    }
}

bool VideoSource::isOpen() const {
    return video_.isOpened(); 
}

cv::Mat VideoSource::grabFrame() {
    cv::Mat frame;
    video_.read(frame);
    return frame;
}