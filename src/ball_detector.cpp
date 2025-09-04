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
    cv::Mat curr(frame.height, frame.width, CV_8UC3,
                frame.data.data(), frame.row_stride);
    
    // int width = widths[VGA];
    // int height = heights[VGA];
    // double scale_x = static_cast<double>(frame.width)  / width;
    // double scale_y = static_cast<double>(frame.height) / height;

    // cv::Mat resized;
    // cv::resize(curr, resized,
    //            cv::Size(widths[VGA], heights[VGA]));

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
    // int x = static_cast<int>(circle[0] * scale_x);
    // int y = static_cast<int>(circle[1] * scale_y);
    // int radius = static_cast<int>(circle[2] * (scale_x + scale_y) / 2.0);

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
