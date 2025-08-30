#pragma once

#include "types.hpp"


class BallDetector {
    public:
        explicit BallDetector();

        bool found_ball() const;

        Ball find_ball(Frame& frame);

        void draw_ball(Frame& frame, const Ball& ball);

    private:
        bool found_ball_;

};