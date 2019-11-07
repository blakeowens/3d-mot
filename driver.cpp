#include <iostream>
#include <vector>
#include <map>

#include "detection.h"
#include "tracker.h"

int main() {

    /*

    currentID = 0000;
    new detection ? currentID += 1
    
    */

    // frames
    std::map<int, std::vector<Detection*> > frames;
    
    // object 1
    Detection* d1_0001 = new Detection(0001, Point(10.0f, 0.0f, 15.0f), Point(0.0f, 0.0f, 0.0f), 0.0f);
    Detection* d2_0001 = new Detection(0001, Point(13.0f, 0.0f, 20.0f), Point(0.0f, 0.0f, 0.0f), 1.0f);

    // frames for object 1
    std::vector<Detection*> d_0001;
    d_0001.push_back(d1_0001);
    d_0001.push_back(d2_0001);

    // object 2
    Detection* d1_0002 = new Detection(0002, Point(-8.0f, 0.0f, -2.0f), Point(0.0f, 0.0f, 0.0f), 0.0f);
    Detection* d2_0002 = new Detection(0002, Point(-17.0f, 0.0f, -6.0f), Point(0.0f, 0.0f, 0.0f), 1.0f);

    // frames for object 2
    std::vector<Detection*> d_0002;
    d_0002.push_back(d1_0002);
    d_0002.push_back(d2_0002);

    // create map of detections
    frames[d1_0001->getID()] = d_0001;
    frames[d1_0002->getID()] = d_0002;

    Tracker* t = new Tracker(frames);

    for (auto a : frames) {
        std::vector<Detection*> curr = frames[a.first];
        for (int i = 0; i < curr.size(); ++i) {
            curr.at(i) -> info();
        }
    }

    return 0;
}