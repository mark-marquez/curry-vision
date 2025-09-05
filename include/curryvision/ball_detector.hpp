#pragma once

#include "types.hpp"


class BallDetector {
    public:
        explicit BallDetector();

        bool found_ball() const;

        Ball find_ball(Frame& frame);

        Ball find_ball_with_smoothing(Frame& frame); // more optimized

        Ball find_ball_hough_transform(Frame& frame); // depracated version

        void draw_ball(Frame& frame, const Ball& ball);

    private:
        bool found_ball_;
        Point last_center{0,0};

};