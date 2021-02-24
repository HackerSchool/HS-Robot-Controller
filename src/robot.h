#ifndef ROBOT_H
#define ROBOT_H

enum {
    DS_LEFT_FRONT,
    DS_RIGHT_FRONT,
    DS_LEFT,
    DS_RIGHT,
    DS_COUNT
};

struct robot;

// Initializes the robot and returns the handle used
struct robot* robot_init(void);

// Terminates the robot using the handle returned by init_robot()
void robot_terminate(struct robot* robot);

// Updates the distances array and sets the motor info from the input variables
// x and y (both go from -1 to 1).
// x - rotation, -1 means rotate left as fast as possible, 1 means rotate right
// as fast as possible, 0 means no rotation at all.
// y - translation, -1 means move back, 1 means move forward, 0 means no
// movement.
// Returns -1 if the program should stop.
int robot_update(struct robot* robot, double x, double y);

// Returns a pointer to an array with the distances measured by the distance
// sensors (array with DS_COUNT elements).
double* robot_get_distances(struct robot* robot);

#endif