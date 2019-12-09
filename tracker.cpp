#include "tracker.h"

Tracker::Tracker() {}

Tracker::~Tracker() {}

/*
    A simple method for reading point cloud data from a text file
*/
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

/*
    Method for matching objects frame-to-frame (frame similarity)
*/
void Tracker::match_objects(void) {
    
    // If there are detections and new points 
    if (detections.size() > 0 && points.size() > 0) {

        // Match the closest point in the new frame's detections to a given detection
        for (auto det : detections) {
            
            // Get the latest detection from the current object
            Detection* curr_det = detections[det.first].at(detections[det.first].size() - 1);

            // Establish a minimum distance and current point
            float min_dist = (float) 0xFFFFFFFF;
            Detection* min_point = NULL;

            // A temporary stack to hold the detections greater than the one with minimum distance
            std::stack<Detection*> temp;
            
            // Find the distance between the detection and each point in the points stack, storing the minimum point
            int size = points.size();
            for (int i = 0; i < size; ++i) {
                
                // Calculate the current distance 
                float curr_dist = distance(curr_det->get_cposition(), points.top()->get_cposition());

                // If this distance is smaller than the current minimum distance
                if (curr_dist < min_dist) {

                    // Add the old minimum to the temp stack, set the new min point and distance, and pop this point from the stack
                    if (min_point != NULL) temp.push(min_point);
                    min_dist = curr_dist; 
                    min_point = points.top();
                    points.pop();

                // If this distance is not smaller than the current minimum distance
                } else {
                    
                    // Push the current point onto the temporary stack and pop it from the points stack
                    temp.push(points.top());
                    points.pop();
                }
            }

            // Copy the remaining points from the temporary stack onto the points stack
            while (points.size() != 0) points.pop();
            while (temp.size() != 0) {
                points.push(temp.top());
                temp.pop();
            }

            // Add the point with minimum distance to the current detection
            min_point->set_id(det.first);
            detections[det.first].push_back(min_point);
        }

        // Push all of the points that did not recieve a match into the unmatched point stack
        while (points.size() != 0) {

            unmatched.push(points.top());
            points.pop();   
        }

    // If there are no detections
    } else if (detections.size() == 0) {
        
        // Push all of the new points into the unmatched stack
        while (points.size() != 0) {
            
            unmatched.push(points.top());
            points.pop();
        }
    } else {
        
        return;
    }
}

/*
    Creates a new detection object for any unmatched obstacle
*/
void Tracker::create_objects(void) {
    
    // If there are unmatched obstacles
    if (unmatched.size() > 0) {
        
        // Create new detections for each obstacle until the unmatched stack is empty
        while (unmatched.size() != 0) {
        
            // Get the current unmatched obstacle
            Detection* d = unmatched.top();
            
            // Give it a new ID
            ++curr_id;
            d->set_id(curr_id);
            
            // Create a new vector of detections
            std::vector<Detection*> ds;
            ds.push_back(d);
            
            // Insert this obstacle into the map and pop it off of the undetected obstacles stack
            detections.insert(std::pair<int, std::vector<Detection*> >(curr_id, ds));
            unmatched.pop();
        }
    } else {
        
        return;
    }
}

/*
    Calculate the distance between two points in 3D space
*/
float Tracker::distance(Point p1, Point p2) {
    
    // Distance between the x, y, and z components of two points
    float x2x1 = p2.x - p1.x;
    float y2y1 = p2.y - p1.y;
    float z2z1 = p2.z - p1.z;

    // Return the distance between p1 and p2
    return std::sqrt((x2x1 * x2x1) + (y2y1 * y2y1) + (z2z1 * z2z1));
}

/*
    Calculate and update the velocities of each current frame in the tracker
*/
void Tracker::update_velocities(void) {
    
    // Calculate the velocity for each detection in the tracker
    for (auto d : detections) {

        // If there are more than two frames for an obstacle, it can be assigned a velocity
        std::vector<Detection*> curr = detections[d.first];
        if (curr.size() >= 2) {
            
            // Get the last two frames of a detection
            Detection* d1 = curr.at(curr.size() - 2);
            Detection* d2 = curr.at(curr.size() - 1);
            
            // Get the positions of the obstacle from these past two frames
            Point p1 = d1->get_cposition();
            Point p2 = d2->get_cposition();

            // Calculate the time difference between the frames
            float time_diff = d2->get_ctime() - d1->get_ctime();

            // Calculate component velocities
            float vx = (p2.x - p1.x) / time_diff;
            float vy = (p2.y - p1.y) / time_diff;
            float vz = (p2.z - p1.z) / time_diff;

            // Set the velocity of the last frame in the detection sequence
            d2->set_velocity(Point(vx, vy, vz));
        }
    }
}
/*
    Makes a linear prediction for each detection based on the objects most recent detected position and currently calculated velocity
*/
void Tracker::make_prediction(void) {

    // Make a prediction for each detection in the tracker
    for (auto a : detections) {
        
        // Get the current list of detections within the map
        std::vector<Detection*> curr = detections[a.first];
        for (int i = 0; i < curr.size(); ++i) {

            // Access the current position and velocity at a given frame
            Detection* d = curr[i];
            Point cpos = d->get_cposition();
            Point vel = (i == 0 && curr.size() > 1) ? curr[i+1]->get_velocity() : d->get_velocity();

            // Set the future time and position of the detection frame
            d->set_ftime(d->get_ctime() + 1.0f);
            d->set_fposition(Point(cpos.x + vel.x, cpos.y + vel.y, cpos.z + vel.z));
        }
    }
}

/*
    Method for printing the tracker results in a formatted outpu
*/
void Tracker::print_detections() {
    
    for (auto a : detections) {
        std::vector<Detection*> curr = detections[a.first];
        std::cout << "ID: " << a.first << std::endl;
        std::cout << "Real Positions: " << std::endl;
        for (auto b : curr) {
            Point pos = b->get_cposition();
            std::cout << std::fixed;
            std::cout << std::setprecision(3);
            std::cout << "(" << pos.x << ", " << pos.y << ", " << pos.z << ") @ " << b->get_ctime() << "s" << std::endl;
        }

        std::cout << "Future Positions: " << std::endl;
        for (auto b : curr) {
            Point pos = b->get_fposition();
            std::cout << std::fixed;
            std::cout << std::setprecision(3);
            std::cout << "(" << pos.x << ", " << pos.y << ", " << pos.z << ") @ " << b->get_ftime() << "s" << std::endl;
        }

        std::cout << "Velocity: " << std::endl;
        for (auto b : curr) {
            Point vel = b->get_velocity();
            std::cout << std::fixed;
            std::cout << std::setprecision(3);
            std::cout << "(" << vel.x << ", " << vel.y << ", " << vel.z << ") @ " << b->get_ctime() << "s" << std::endl;
        }
        std::cout << std::endl;
    }
}