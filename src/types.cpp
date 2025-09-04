#include <cstdint>
#include <vector>
#include <cmath>
#include <utility>
#include <curryvision/types.hpp>


Point::Point() {}
Point::Point(int init_x, int init_y) : x_(init_x), y_(init_y) {}

int Point::get_x() const {
    return x_;
}
int Point::get_y() const {
    return y_;
}

void Point::set_x(int new_x) {
    x_ = new_x;
}
void Point::set_y(int new_y) {
    y_ = new_y;
}

float Point::distance_to(const Point& point) const {
    float x_diff = point.get_x() - x_;
    float y_diff = point.get_y() - y_;
    float x_sqrd = std::pow(x_diff, 2);
    float y_sqrd = std::pow(y_diff, 2);
    float distance = (float)(std::sqrt(x_sqrd + y_sqrd));
    return distance;
}


Box::Box() {}
Box::Box(int x, int y, int radius) {
    top_left_ = Point(x - radius, y + radius);
    top_right_ = Point(x + radius, y + radius);
    bottom_left_ = Point(x - radius, y - radius);
    bottom_right_ = Point(x + radius, y - radius);
}
Box::Box(Point top_left, Point top_right, Point bottom_left, Point bottom_right) :
top_left_(top_left), top_right_(top_right), bottom_left_(bottom_left), bottom_right_(bottom_right) {}

Point Box::get_top_left() const {
    return top_left_;
}
Point Box::get_top_right() const {
    return top_right_;
}
Point Box::get_bottom_left() const {
    return bottom_left_;
}
Point Box::get_bottom_right() const {
    return bottom_right_;
}

void Box::set_top_left(Point point) {
    top_left_ = point;
}
void Box::set_top_right(Point point) {
    top_right_ = point;
}
void Box::set_bottom_left(Point point) {
    bottom_left_ = point;
}
void Box::set_bottom_right(Point point) {
    bottom_right_ = point; 
}


void FrameQueue::push(Frame frame) {
    {
        std::lock_guard<std::mutex> lock(m_); // automatically releases mutex
        q_.push(std::move(frame)); // pointer swap
    }
    cv_.notify_one();
}

Frame FrameQueue::pop() {
    std::unique_lock<std::mutex> lock(m_); // required to use wait
    cv_.wait(lock, [this]{ return !q_.empty(); }); // sleep until notified & queue has a frame
    Frame frame = std::move(q_.front());
    q_.pop();
    return frame;
}

bool FrameQueue::empty() const {
    std::lock_guard<std::mutex> lock(m_); // automatically releases mutex
    return q_.empty();
}


void LatestFrame::set_frame(Frame f) {
    {
        std::lock_guard<std::mutex> lock(m_);
        latest_frame_ = std::move(f);
    }
    cv_.notify_one();
}

std::optional<Frame> LatestFrame::get_frame() {
    std::unique_lock<std::mutex> lock(m_);
    cv_.wait(lock, [this]{
        return latest_frame_.has_value() || stop_;
    });

    if (!latest_frame_) {
        return std::nullopt;
    }

    auto out = std::move(*latest_frame_);
    latest_frame_.reset();
    return out;
}

void LatestFrame::set_ball(Ball b) {
    std::lock_guard<std::mutex> lock(m_);
    latest_ball_ = std::move(b);
    latest_ball_ts_ = std::chrono::steady_clock::now();
}

std::optional<Ball> LatestFrame::try_get_ball() {
    std::lock_guard<std::mutex> lock(m_);
    if (!latest_ball_) return std::nullopt;

    auto now = std::chrono::steady_clock::now();
    if (now - latest_ball_ts_ > BALL_TTL) {
        latest_ball_.reset();
        latest_ball_ts_ = {};
        return std::nullopt;
    }
    return *latest_ball_;
}

void LatestFrame::stop() {
    std::lock_guard<std::mutex> lock(m_);
    stop_ = true;
    cv_.notify_one();
}