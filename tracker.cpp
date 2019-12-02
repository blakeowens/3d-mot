#include "tracker.h"

Tracker::Tracker() {

}

Tracker::~Tracker() {}

void Tracker::read_data(std::string filename, int num_frames) {

    std::string path = "data/" + filename;

    std::fstream file;
    file.open(path);

    if (!file)
        std::cout << "Could not locate frame" << std::endl;
    
    int frame_num;
    file >> frame_num;

    float time = (1 / (float) num_frames) * (float) frame_num;

    while(!file.eof()) {
        
        float x, y, z;
        file >> x;
        file >> y;
        file >> z;

        Detection* d = new Detection(0, Point(x, y, z), Point(), time);
        points.push(d);
    }
}

void Tracker::match_objects(void) {
    
    if (detections.size() > 0 && points.size() > 0) {
        
        bool new_detections = true;
        if (points.size() == detections.size())
            new_detections = false;
        
        while (points.size() != 0) {
            
            int min_id;
            float min_dist = (float) 0xFFFFFFFF;
            
            for (auto det : detections) {
                
                Detection* d = detections[det.first].at(detections[det.first].size() - 1);
                float curr_dist = distance(d->get_position(), points.top()->get_position());
                if (curr_dist < min_dist) {
                    
                    min_dist = curr_dist;
                    min_id = det.first;
                }
            }

            if (!new_detections) {
                Detection* d = points.top();
                d->set_id(min_id);
                detections[min_id].push_back(d);
                points.pop();
            } else if (new_detections) {
                // check velocities here
            } else {
                unmatched.push(points.top());
                points.pop();
            }
        }

        // for each point in the points stack
            // get the detection with the closest distance
            // if the number of points is equal to the number of detections (no new detections), make match
                // remove the detection from points, add it to the map
            // if the number of points is not equal to the number of detections
                // check the velocity directions with the minimum distance match
                    // if the distance is within the minimum threshold, match
                        // remove the detection from points, add it to the map
            // if the point fails all the above tests, add it to unmatched
    } else if (detections.size() == 0) {
        
        while (points.size() != 0) {
            
            unmatched.push(points.top());
            points.pop();
        }
    } else {
        
        return;
    }
}

void Tracker::create_objects(void) {
    
    if (unmatched.size() > 0) {
        
        while (unmatched.size() != 0) {
        
            Detection* d = unmatched.top();
            
            ++curr_id;
            d->set_id(curr_id);
            
            std::vector<Detection*> ds;
            ds.push_back(d);
            
            detections.insert(std::pair<int, std::vector<Detection*> >(curr_id, ds));
            unmatched.pop();
        }
    } else {
        
        return;
    }
}

float Tracker::distance(Point p1, Point p2) {
    
    float x2x1 = p2.x - p1.x;
    float y2y1 = p2.y - p1.y;
    float z2z1 = p2.z - p1.z;

    float d = std::sqrt((x2x1 * x2x1) + (y2y1 * y2y1) + (z2z1 * z2z1));

    return d;
}

void Tracker::print_points() {
    
    std::stack<Detection*> points_cpy = points;
    while (points_cpy.size() != 0) {
        Detection* d = points_cpy.top();
        d->info();
        points_cpy.pop();
    }
}

void Tracker::print_detections() {
    
    for (auto a : detections) {
        std::vector<Detection*> curr = detections[a.first];
        std::cout << "ID: " << a.first << std::endl;
        std::cout << "Positions: " << std::endl;
        for (auto b : curr) {
            Point pos = b->get_position();
            std::cout << std::fixed;
            std::cout << std::setprecision(3);
            std::cout << "(" << pos.x << ", " << pos.y << ", " << pos.z << ") @ " << b->get_time() << "s" << std::endl;
        }
        std::cout << "Velocity: " << std::endl;
        for (auto b : curr) {
            Point vel = b->get_velocity();
            std::cout << std::fixed;
            std::cout << std::setprecision(3);
            std::cout << "(" << vel.x << ", " << vel.y << ", " << vel.z << ") @ " << b->get_time() << "s" << std::endl;
        }
        std::cout << std::endl;
    }
}