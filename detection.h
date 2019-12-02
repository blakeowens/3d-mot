#ifndef DETECTION_H
#define DETECTION_H

struct Point {
    float x, y, z;

    Point() : x(0.0f), y(0.0f), z(0.0f) {}
    Point(float x, float y, float z) : x(x), y(y), z(z) {}
};

class Detection {
    private:
        float ctime, ftime;
        Point cp, fp, v;
        int id;
    public:
        Detection(int, Point, Point, float);
        ~Detection();

        void info();

        int get_id();
        Point get_cposition();
        Point get_fposition();
        Point get_velocity();
        float get_ctime();
        float get_ftime();

        void set_id(int);
        void set_velocity(Point);
        void set_fposition(Point);
        void set_ftime(float);
};

#endif