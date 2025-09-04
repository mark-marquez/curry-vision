#pragma once

#include <queue>
#include <mutex>
#include <condition_variable>

static constexpr int QVGA = 0;
static constexpr int VGA = 1; 
static constexpr int HD = 2;
static constexpr int FULLHD = 3;

const std::vector<int> widths = {320, 640, 1280, 1920};
const std::vector<int> heights = {240, 480, 720, 1080};

struct Frame {
    std::uint64_t id;
    std::uint64_t timestamp_ms;
    int width;
    int height;
    int row_stride;
    std::vector<unsigned char> data; // raw pixel buffer
};


class Point {
    public:
        Point();
        Point(int init_x, int init_y);

        int get_x() const;
        int get_y() const;

        void set_x(int new_x);
        void set_y(int new_y);

        float distance_to(const Point& point) const; 

    private:
        int x_;
        int y_;
};


class Box {
    public:
        Box();
        Box(int x, int y, int radius);
        Box(Point top_left, Point top_right, Point bottom_left, Point bottom_right);

        Point get_top_left() const;
        Point get_top_right() const;
        Point get_bottom_left() const;
        Point get_bottom_right() const;

        void set_top_left(Point point);
        void set_top_right(Point point);
        void set_bottom_left(Point point);
        void set_bottom_right(Point point);

    private:
        Point top_left_;
        Point top_right_;
        Point bottom_left_;
        Point bottom_right_;
};


struct Ball {
    Point center;
    Box bbox; // Bounding Box
};


class FrameQueue { // Producer & consumer pattern
public:
    void push(Frame frame);
    Frame pop();
    bool empty() const;

private:
    mutable std::mutex m_;
    std::queue<Frame> q_;
    std::condition_variable cv_;
};