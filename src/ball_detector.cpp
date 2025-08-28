#pragma once

#include "curryvision/ball_detector.hpp"


BallDetector::BallDetector() : found_ball_(false) {}

bool BallDetector::found_ball() const {
    return found_ball_;
}

Ball find_ball(Frame& frame) {
    
}