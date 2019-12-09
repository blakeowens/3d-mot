#include <iostream>
#include <vector>
#include <map>
#include <string>

#include "detection.h"
#include "tracker.h"

int main() {

    std::vector<std::string> files {"frame_0.txt", "frame_1.txt", "frame_2.txt", "frame_3.txt", "frame_4.txt", "frame_5.txt"};

    Tracker* d = new Tracker();
 
    for (auto s : files) {
        d->read_data(s, files.size());
        d->match_objects();
        d->create_objects();
        d->update_velocities();
        d->make_prediction();
    }
    
    d->print_detections();

    return 0;
}

/*

Tracker::process(std::stack<Detection*> points) {
    this->points = points;
    match_objects();
    create_objects();
    update_velocities();
    make_prediction();   
}

*/
