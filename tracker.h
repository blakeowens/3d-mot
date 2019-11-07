#ifndef TRACKER_H
#define TRACKER_H

#include <vector>
#include <map>
#include "detection.h"

class Tracker {
    private:
        std::map<int, std::vector<Detection*> > frames;
        Point vehicle(0.0f, 0.0f, 0.0f);
        float dist_threshold; // radius
        float vel_threshold; // velocity threshold

    public:
        Tracker(std::map<int, std::vector<Detection*> >);
        ~Tracker();

        // void cal_velocity(vector<detection*>)
};

#endif