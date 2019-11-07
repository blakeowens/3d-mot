#ifndef DETECTION_H
#define DETECTION_H

struct Point {
    float x, y, z;

    Point(float x, float y, float z) : x(x), y(y), z(z) {}
};

class Detection {
    private:
        float time;
        Point p;
        Point v;
        int id;
    public:
        Detection(int, Point, Point, float);
        ~Detection();

        void info();
        int getID();

        // set velocity

};

#endif