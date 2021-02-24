#include "robot.h"

#include <stdio.h>

int main() {
    struct robot* robot = robot_init();
    if (robot == NULL) {
        perror("robot_init() failed");
        return 1;
    }

    double move_x = 1.0, move_y = 1.0;
    while (robot_update(robot, move_x, move_y) != -1) {
        // To Do: calculate move_x & move_y
    }

    robot_terminate(robot);
    return 0;
}
