#ifndef TRACKER_H
#define TRACKER_H

#include <vector>
#include <stack>
#include <string>
#include <map>
#include <fstream>
#include <iostream>
#include <cmath>
#include <iomanip>
#include "detection.h"

class Tracker {
    private:
        int curr_id = 0;
        std::map<int, std::vector<Detection*> > detections;
        std::stack<Detection*> points, unmatched;
        const int FRAME_RATE = 6;
    public:
        Tracker();
        ~Tracker();

        void read_data(std::string, int);
        void match_objects(void);
        void create_objects(void);

        float distance(Point, Point);

        void update_velocities(void);
        void make_prediction(void);
        
        void print_points(void);
        void print_detections(void);

};

#endif