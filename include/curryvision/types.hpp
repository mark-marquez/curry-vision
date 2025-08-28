#pragma once

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
        int x;
        int y;
};

class Box {
    public:
        Box();

        Box(Point top_left, Point top_right, Point bottom_left, Point bottom_right);

        int get_x() const;

        int get_y() const;

        void set_x(int new_x);

        void set_y(int new_y);

        float distance_to(const Point& point) const; 

    private:
        int x;
        int y;
};