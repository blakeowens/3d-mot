#include "tracker.h"

Tracker::Tracker(std::map<int, std::vector<Detection*> > frames) {
    
    this->frames = frames;
}

Tracker::~Tracker() {

}