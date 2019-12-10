#include <iostream>
#include <vector>
#include <map>
#include <string>

#include "detection.h"
#include "tracker.h"

int main() {

    std::vector<std::string> files {"frame_0.txt", "frame_1.txt", "frame_2.txt", "frame_3.txt", "frame_4.txt", "frame_5.txt", "frame_6.txt", "frame_7.txt", "frame_8.txt"};

    Tracker* d = new Tracker();
 
    for (auto s : files) {
        d->read_data(s, files.size());
        std::cout << "data read" << std::endl;
        d->match_objects();
        std::cout << "objects matched" << std::endl;
        d->create_objects();
        std::cout << "objects created" << std::endl;
        d->update_velocities();
        std::cout << "velocities updated" << std::endl;
        d->make_prediction();
        std::cout << "predictions made" << std::endl;
    }
    
    d->print_detections();

    return 0;
}