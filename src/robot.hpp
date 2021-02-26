#ifndef ROBOT_H
#define ROBOT_H

#include <opencv2/opencv.hpp>

enum {
    DS_LEFT_FRONT,
    DS_RIGHT_FRONT,
    DS_LEFT,
    DS_RIGHT,
    DS_COUNT
};

struct Robot;

// Initializes the robot and returns the handle used
Robot* robot_init();

// Terminates the robot using the handle returned by robot_init()
void robot_terminate(Robot* robot);

// Updates the distances array and sets the motor info from the input variables
// x and y (both go from -1 to 1).
// x - rotation, -1 means rotate left as fast as possible, 1 means rotate right
// as fast as possible, 0 means no rotation at all.
// y - translation, -1 means move back, 1 means move forward, 0 means no
// movement.
// Returns -1 if the program should stop.
int robot_update(Robot* robot, double x, double y, double z);

// Returns a pointer to an array with the distances measured by the distance
// sensors (array with DS_COUNT elements).
double* robot_get_distances(Robot* robot);

// Returns the image data recorded by the robot's camera (BGRA)
cv::Mat robot_get_image(Robot* robot);

#endif