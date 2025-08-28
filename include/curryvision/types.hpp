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

        Point get_top_left() const;
        Point get_top_right() const;
        Point get_bottom_left() const;
        Point get_bottom_right() const;

        void set_top_left(Point point);
        void set_top_right(Point point);
        void set_bottom_left(Point point);
        void set_bottom_right(Point point);

    private:
        Point top_left;
        Point top_right;
        Point bottom_left;
        Point bottom_right;
};