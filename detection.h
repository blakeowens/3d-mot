#ifndef DETECTION_H
#define DETECTION_H

struct Point {
    float x, y, z;

    Point() : x(0.0f), y(0.0f), z(0.0f) {}
    Point(float x, float y, float z) : x(x), y(y), z(z) {}
};

class Detection {
    private:
        float ctime, ftime, l, w, h;
        Point cp, fp, v;
        int id;
    public:
        Detection(int id, Point cp, Point fp, float ctime) { this->id = id; this->cp = cp; this->fp = fp; this->ctime = ctime; }
        ~Detection();

        int get_id() { return id; }
        Point get_cposition() { return cp; }
        Point get_fposition() { return fp; }
        Point get_velocity() { return v; }
        float get_ctime() { return ctime; }
        float get_ftime() { return ftime; }

        void set_id(int id) { this->id = id; }
        void set_velocity(Point v) { this->v = v; } 
        void set_fposition(Point fp) { this->fp = fp; }
        void set_ftime(float ftime) { this->ftime = ftime; }
};

#endif