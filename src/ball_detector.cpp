#include <vector>
#include "curryvision/ball_detector.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"
//#include "opencv2/highgui.hpp"


BallDetector::BallDetector() : found_ball_(false) {}

bool BallDetector::found_ball() const {
    return found_ball_;
}

Ball BallDetector::find_ball(Frame& frame) {
    if (frame.width == 0 || frame.height == 0 || frame.data.empty()) {
        return Ball{ Point(0,0), Box(0,0,0) };
    }
    cv::Mat curr(frame.height, frame.width, CV_8UC3,
            frame.data.data(), frame.row_stride);
    cv::Mat blur;
    cv::GaussianBlur(curr, blur, cv::Size(11, 11), 0);
    cv::Mat hsv;
    cv::cvtColor(blur, hsv, cv::COLOR_BGR2HSV);
    cv::Mat mask;
    cv::inRange(hsv, cv::Scalar(5, 120, 120), cv::Scalar(25, 255, 255), mask);
    cv::erode(mask, mask, cv::Mat(), cv::Point(-1,-1), 1);
    cv::dilate(mask, mask, cv::Mat(), cv::Point(-1,-1), 1);
    
    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(mask, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
    if (contours.empty()) return Ball{ Point(0,0), Box(0,0,0) };

    auto& c = *std::max_element(contours.begin(), contours.end(),
        [](const auto& a, const auto& b){ return cv::contourArea(a) < cv::contourArea(b); });

    cv::Point2f ctr; float r;
    cv::minEnclosingCircle(c, ctr, r);

    return Ball{ Point((int)ctr.x, (int)ctr.y), Box((int)ctr.x, (int)ctr.y, (int)r) };

}

Ball BallDetector::find_ball_hough_transform(Frame& frame) {
    cv::Mat curr(frame.height, frame.width, CV_8UC3,
                frame.data.data(), frame.row_stride);

    cv::Mat gray;
    cv::cvtColor(curr, gray, cv::COLOR_BGR2GRAY);
    cv::medianBlur(gray, gray, 5);

    std::vector<cv::Vec3f> circles;
    cv::HoughCircles(gray, circles, cv::HOUGH_GRADIENT, 1,
                     gray.rows/16,
                     100, 30, 1, 200);

    cv::Vec3i circle = circles[0]; 
    int x = circle[0];
    int y = circle[1];
    int radius = circle[2];
    Point center(x, y);
    Box bbox(x, y, radius);
    Ball ball { center, bbox };

    return ball;
}

void BallDetector::draw_ball(Frame& frame, const Ball& ball) {
    cv::Mat img(frame.height, frame.width, CV_8UC3,
                frame.data.data(), frame.row_stride);

    // Draw center
    cv::circle(img,
               cv::Point(ball.center.get_x(), ball.center.get_y()),
               3,                   // small radius
               cv::Scalar(0, 0, 255), // red
               -1);                 // filled

    // Draw bounding box as rectangle (top-left to bottom-right)
    cv::rectangle(img,
                  cv::Point(ball.bbox.get_top_left().get_x(),
                            ball.bbox.get_top_left().get_y()),
                  cv::Point(ball.bbox.get_bottom_right().get_x(),
                            ball.bbox.get_bottom_right().get_y()),
                  cv::Scalar(0, 255, 0), // green
                  2);                    // line thickness
}
