#pragma once

#include <vector>
#include "types.hpp"


class BallDetector {
    public:
        explicit BallDetector();
        ~BallDetector();

        bool found_ball() const;

        Ball find_ball(Frame& frame);

    private:
        bool found_ball_;
};