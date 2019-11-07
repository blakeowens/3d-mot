#ifndef DETECTION_H
#define DETECTION_H

struct Point {
    float x, y, z;

    Point() : x(0), y(0), z(0) {}
    Point(float x, float y, float z) : x(x), y(y), z(z) {}
};

class Detection {
    private:
        float time;
        Point p, v; 
        int id;
    public:
        Detection(int, Point, Point, float);
        ~Detection();

        void info();
        int getID();

        // set velocity

};

#endif