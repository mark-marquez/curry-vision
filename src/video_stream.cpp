#include <chrono>
#include <stdexcept>
#include <opencv2/opencv.hpp>
#include "curryvision/video_stream.hpp"


cv::VideoCapture capture; 
int camera_index;
int width; 
int height;
const std::string window_name = "CurryVision";


VideoStream::VideoStream(int cam_idx, int resolution) : running_(false), display_enabled_(false), frame_counter_(0) {
    camera_index = cam_idx; 
    width = widths[resolution];
    height = heights[resolution];
}

VideoStream::~VideoStream() {
    stop(); 
}

bool VideoStream::start() {
    if (running_) return true;
    if (!capture.open(camera_index)) return false;
    capture.set(cv::CAP_PROP_FRAME_WIDTH,  width);
    capture.set(cv::CAP_PROP_FRAME_HEIGHT, height);
    running_ = true;
    return true;
}

void VideoStream::stop() {
    if (!running_) return;
    running_ = false;
    if (capture.isOpened()) capture.release();
    if (display_enabled_) {
        cv::destroyWindow(window_name);
        display_enabled_ = false;
    }
}

bool VideoStream::is_running() const {
    return running_;
}

void VideoStream::show(bool enable) {
    if (enable && !display_enabled_) {
        cv::namedWindow(window_name, cv::WINDOW_NORMAL);
        display_enabled_ = true;
    } else if (!enable && display_enabled_) {
        cv::destroyWindow(window_name);
        display_enabled_ = false;
    }
}

Frame VideoStream::get_frame() {
    Frame frame{};
    if (!running_ || !capture.isOpened()) return frame;

    cv::Mat bgr;
    if (!capture.read(bgr) || bgr.empty()) return frame;

    std::uint64_t ms = std::chrono::duration_cast<std::chrono::milliseconds>(
                  std::chrono::steady_clock::now().time_since_epoch())
                  .count();

    frame.id = ++frame_counter_;
    frame.timestamp_ms = static_cast<std::uint64_t>(ms);
    frame.width = bgr.cols;
    frame.height = bgr.rows;
    frame.row_stride = bgr.step;            
    frame.data.assign(bgr.datastart, bgr.dataend);

    return frame;
}

void VideoStream::display(const Frame& frame) {
    if (!display_enabled_) return;

    cv::Mat img(frame.height, frame.width, CV_8UC3,
                const_cast<unsigned char*>(frame.data.data()), frame.row_stride);

    cv::imshow(window_name, img);
    cv::waitKey(1);
}
