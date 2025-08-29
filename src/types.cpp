#pragma once

#include <cstdint>
#include <vector>
#include <cmath>
#include <curryvision/types.hpp>


class Point {
    public:
        Point() {}
        Point(int init_x, int init_y) : x_(init_x), y_(init_y) {}

        int get_x() const {
            return x_;
        }
        int get_y() const {
            return y_;
        }

        void set_x(int new_x) {
            x_ = new_x;
        }
        void set_y(int new_y) {
            y_ = new_y;
        }

        float distance_to(const Point& point) const {
            float x_diff = point.get_x() - x_;
            float y_diff = point.get_y() - y_;
            float x_sqrd = std::pow(x_diff, 2);
            float y_sqrd = std::pow(y_diff, 2);
            float distance = (float)(std::sqrt(x_sqrd + y_sqrd));
            return distance;
        }

    private:
        int x_;
        int y_;
};


class Box {
    public:
        Box();
        Box(int x, int y, int radius) {
            top_left_ = Point(x - radius, y + radius);
            top_right_ = Point(x + radius, y + radius);
            bottom_left_ = Point(x - radius, y - radius);
            bottom_right_ = Point(x + radius, y - radius);
        }
        Box(Point top_left, Point top_right, Point bottom_left, Point bottom_right) :
        top_left_(top_left), top_right_(top_right), bottom_left_(bottom_left), bottom_right_(bottom_right) {}

        Point get_top_left() const {
            return top_left_;
        }
        Point get_top_right() const {
            return top_right_;
        }
        Point get_bottom_left() const {
            return bottom_left_;
        }
        Point get_bottom_right() const {
            return bottom_right_;
        }

        void set_top_left(Point point) {
            top_left_ = point;
        }
        void set_top_right(Point point) {
            top_right_ = point;
        }
        void set_bottom_left(Point point) {
            bottom_left_ = point;
        }
        void set_bottom_right(Point point) {
            bottom_right_ = point; 
        }

    private:
        Point top_left_;
        Point top_right_;
        Point bottom_left_;
        Point bottom_right_;
};