#pragma once

#include <vector>
#include "curryvision/ball_detector.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"
//#include "opencv2/highgui.hpp"


BallDetector::BallDetector() : found_ball_(false) {}

bool BallDetector::found_ball() const {
    return found_ball_;
}

Ball find_ball(Frame& frame) {
    // Will use OpenCV's HaughCircles implementation
    // Will be much more performant than my own implementation
    Ball ball;

    cv::Mat curr(frame.height, frame.width, CV_8UC3,
                frame.data.data(), frame.row_stride);

    cv::Mat gray;
    cv::cvtColor(curr, gray, cv::COLOR_BGR2GRAY);
    cv::medianBlur(gray, gray, 5);

    std::vector<cv::Vec3f> circles;
    cv::HoughCircles(gray, circles, cv::HOUGH_GRADIENT, 1,
                     gray.rows/16,
                     100, 30, 1, 50);

    cv::Vec3i circle = circles[0]; 
    int x = circle[0];
    int y = circle[1];
    int radius = circle[2];

    Point top_left = Point(x - radius, y + radius);
    Point top_right = Point(x + radius, y + radius);
    Point bottom_left = Point(x - radius, y - radius);
    Point bottom_right = Point(x + radius, y - radius);

    Box bbox(top_left, top_right, bottom_left, bottom_right);
    Point bottom_left = Point(x - radius, y - radius);
    



    

}