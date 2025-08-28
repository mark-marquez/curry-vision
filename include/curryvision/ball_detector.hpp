#pragma once

#include <vector>
#include <types.hpp>

class BallDetector {
    public:
        explicit BallDetector();
        ~BallDetector();

        bool is_present() const;

        struct Ball {
            Point center;
            Point top_left;
            Point top_right;
            Point bottom_left;
            Point bottom_right;
        };

    private:

};